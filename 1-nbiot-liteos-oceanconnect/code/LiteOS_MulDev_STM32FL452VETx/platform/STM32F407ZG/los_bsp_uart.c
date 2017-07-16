#include "los_bsp_uart.h"
#include <stdarg.h>

#include "los_sem.h"
#include "los_base.ph"
#include "los_hwi.h"
#include "los_api_sem.h"


#ifdef LOS_STM32F407ZG
#include "stm32f4xx_hal.h"

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef UartHandle;

static UINT32 g_uartrSemID;

static char uartfifo[2][1056];
static int curbuf = 0;

#define LOS_UART_IT 1

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

static HAL_StatusTypeDef UART_Receive_IT2(UART_HandleTypeDef *huart)
{
  uint16_t* tmp;
  
  /* Check that a Rx process is ongoing */
  if(huart->RxState == HAL_UART_STATE_BUSY_RX) 
  {
    if(huart->Init.WordLength == UART_WORDLENGTH_9B)
    {
      tmp = (uint16_t*) huart->pRxBuffPtr;
      if(huart->Init.Parity == UART_PARITY_NONE)
      {
        *tmp = (uint16_t)(huart->Instance->DR & (uint16_t)0x01FF);
        huart->pRxBuffPtr += 2U;
      }
      else
      {
        *tmp = (uint16_t)(huart->Instance->DR & (uint16_t)0x00FF);
        huart->pRxBuffPtr += 1U;
      }
    }
    else
    {
      if(huart->Init.Parity == UART_PARITY_NONE)
      {
        *huart->pRxBuffPtr++ = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
      }
      else
      {
        *huart->pRxBuffPtr++ = (uint8_t)(huart->Instance->DR & (uint8_t)0x007F);
      }
    }
    if (huart->RxXferSize - huart->RxXferCount >= 500)
    {
        LOS_SemPost(g_uartrSemID);
    }
    
    if(--huart->RxXferCount == 0U)
    {
 #if 0
      /* Disable the UART Parity Error Interrupt and RXNE interrupt*/
      CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));

      /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
      CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

      /* Rx process is completed, restore huart->RxState to Ready */
      huart->RxState = HAL_UART_STATE_READY;
     
      HAL_UART_RxCpltCallback(huart);
#endif
      return HAL_OK;
    }
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

void HAL_UART_IRQHandler2(UART_HandleTypeDef *huart)
{
   uint32_t isrflags   = READ_REG(huart->Instance->SR);
   uint32_t cr1its     = READ_REG(huart->Instance->CR1);
   uint32_t cr3its     = READ_REG(huart->Instance->CR3);
   uint32_t errorflags = 0x00U;
   uint32_t dmarequest = 0x00U;
   uint16_t tmp;
  /* If no error occurs */
  errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
  if(errorflags == RESET)
  {
    /* UART in mode Receiver -------------------------------------------------*/
    if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
    {
      UART_Receive_IT2(huart);
      return;
    }
  }  

  /* If some errors occur */
  if((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))
  {
    /* UART parity error interrupt occurred ----------------------------------*/
    if(((isrflags & USART_SR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))
    {
      huart->ErrorCode |= HAL_UART_ERROR_PE;
    }
    
    /* UART noise error interrupt occurred -----------------------------------*/
    if(((isrflags & USART_SR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
    {
      huart->ErrorCode |= HAL_UART_ERROR_NE;
    }
    
    /* UART frame error interrupt occurred -----------------------------------*/
    if(((isrflags & USART_SR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
    {
      huart->ErrorCode |= HAL_UART_ERROR_FE;
    }
    
    /* UART Over-Run interrupt occurred --------------------------------------*/
    if(((isrflags & USART_SR_ORE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
    { 
      huart->ErrorCode |= HAL_UART_ERROR_ORE;
    }

    /* Call UART Error Call back function if need be --------------------------*/    
    if(huart->ErrorCode != HAL_UART_ERROR_NONE)
    {
      /* UART in mode Receiver -----------------------------------------------*/
      if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
      {
        UART_Receive_IT2(huart);
      }

      /* If Overrun error occurs, or if any error occurs in DMA mode reception,
         consider error as blocking */
      dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);
      if(((huart->ErrorCode & HAL_UART_ERROR_ORE) != RESET) || dmarequest)
      {
        /* Blocking error : transfer is aborted
           Set the UART state ready to be able to start again the process,
           Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
        //UART_EndRxTransfer(huart);
        tmp = (uint16_t)(huart->Instance->DR & (uint16_t)0x00FF);
        /* Disable the UART DMA Rx request if enabled */
      }
      else
      {
        /* Non Blocking error : transfer could go on. 
           Error is notified to user through user error callback */
        HAL_UART_ErrorCallback(huart);
        huart->ErrorCode = HAL_UART_ERROR_NONE;
      }
    }
    return;
  } /* End if some error occurs */

  /* UART in mode Transmitter ------------------------------------------------*/
  if(((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
  {
    //UART_Transmit_IT(huart);
    return;
  }
  
  /* UART in mode Transmitter end --------------------------------------------*/
  if(((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
  {
    //UART_EndTransmit_IT(huart);
    return;
  }
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

/* UART4 init function */
static void MX_UART4_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* UART5 init function */
static void MX_UART5_Init(void)
{

  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Pinout Configuration
*/
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

}

void LOS_EvbUart_ReadBytes(char *rbuf, int *readlen)
{
    UINT32 uwRet;
    int bufidx = curbuf;
    
    uwRet = LOS_SemPend(g_uartrSemID, 10);
    if(LOS_OK == uwRet)
    {
        *readlen = 0;
    }
    if(LOS_ERRNO_SEM_TIMEOUT == uwRet)
    {
        //timeout
        if (UartHandle.RxXferCount != UartHandle.RxXferSize)
        {
            curbuf = (curbuf+1)%2;
            *readlen = UartHandle.RxXferSize - UartHandle.RxXferCount;
            UartHandle.pRxBuffPtr = (uint8_t*)uartfifo[curbuf];
            UartHandle.RxXferCount = UartHandle.RxXferSize;
            memcpy(rbuf, uartfifo[bufidx],*readlen);
        }
    }
    return ;
}

#endif

void LOS_EvbUartInit(void)
{
#ifdef LOS_STM32F407ZG
    MX_GPIO_Init();
    MX_UART4_Init();
    MX_UART5_Init();
    UartHandle = huart4;
    
    LOS_SemCreate(0,&g_uartrSemID);
#endif
    
    return ;
}

/*************************************************************************************************
 *  功能：向串口1发送一个字符                                                                    *
 *  参数：(1) 需要被发送的字符                                                                   *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartWriteByte(char c)
{
#ifdef LOS_STM32F746ZG
#ifdef LOS_UART_IT
  if(HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)&c, 1)!= HAL_OK)
  {
    Error_Handler();
  }
#else
	if(HAL_UART_Transmit(&UartHandle, (uint8_t*)&c, 1, 5000)!= HAL_OK)
	{
		Error_Handler();   
	}
#endif
#endif
	return ;
}

/*************************************************************************************************
 *  功能：向串口1发送一个字符串                                                                  *
 *  参数：(1) 需要被发送的字符串                                                                 *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartWriteStr(const char* str)
{
	
#ifdef LOS_STM32F746ZG
    HAL_StatusTypeDef ret;
    while (*str)
    {
#ifdef LOS_UART_IT
			ret = HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)&str, 1);
			if(ret != HAL_OK)
			{
				Error_Handler();
			}
#else
			if(HAL_UART_Transmit(&UartHandle, (uint8_t*)str, 1, 5000)!= HAL_OK)
			{
				Error_Handler();   
			}
#endif
			str++;
    }
#endif
	return ;
}

void LOS_EvbUartWriteStr2(const char* str, unsigned short len)
{
	
#ifdef LOS_STM32F746ZG
    HAL_StatusTypeDef ret;

#ifdef LOS_UART_IT
    /*
			ret = HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)&str, len);
			if(ret != HAL_OK)
			{
				Error_Handler();
			}
    */
			if(HAL_UART_Transmit(&UartHandle, (uint8_t*)str, len, 5000)!= HAL_OK)
			if(ret != HAL_OK)
			{
				Error_Handler();
			}
#else
			if(HAL_UART_Transmit(&UartHandle, (uint8_t*)str, len, 5000)!= HAL_OK)
			{
				Error_Handler();   
			}
#endif

#endif
	return ;
}

/*************************************************************************************************
 *  功能：从串口1接收一个字符                                                                    *
 *  参数：(1) 存储接收到的字符                                                                   *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartReadByte(char* c)
{
#ifdef LOS_STM32F746ZG
#ifdef LOS_UART_IT
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)c, 1) != HAL_OK)
  {
    Error_Handler();
  }
#else
	if(HAL_UART_Receive(&UartHandle, (uint8_t *)c, 1, 5000) != HAL_OK)
	{
		Error_Handler();  
	}
#endif
#endif
	return ;
}

void LOS_EvbUartStartReadIT(unsigned short len)
{
#ifdef LOS_STM32F746ZG
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)uartfifo[0], len) != HAL_OK)
  {
    Error_Handler();
  }
#endif
	return ;
}

void LOS_EvbUartReadByte2(char* c, unsigned short len, unsigned int timeout)
{
#ifdef LOS_STM32F746ZG
#ifdef LOS_UART_IT
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)c, len) != HAL_OK)
  {
    Error_Handler();
  }
#else
	if(HAL_UART_Receive(&UartHandle, (uint8_t *)c, len, timeout) != HAL_OK)
	{
		//Error_Handler();  
	}
#endif
#endif
	return ;
}

#ifdef LOS_STM32F407ZG
static char _buffer[128];
#endif
void LOS_EvbUartPrintf(char* fmt, ...)
{
#ifdef LOS_STM32F407ZG
    int i;
    va_list ap;
    va_start(ap, fmt);
    vsprintf(_buffer, fmt, ap);
    va_end(ap);

    for (i = 0; _buffer[i] != '\0'; i++)
    {
        LOS_EvbUartWriteByte(_buffer[i]);
    }
#endif
    return ;
}
