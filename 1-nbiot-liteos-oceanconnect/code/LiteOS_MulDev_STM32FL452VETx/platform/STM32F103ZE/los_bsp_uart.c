#include "los_bsp_uart.h"
#include <stdarg.h>

#include "los_sem.h"
#include "los_base.ph"
#include "los_hwi.h"
#include "los_api_sem.h"

#ifdef LOS_STM32F103ZETx
#include "stm32f1xx_hal.h"


UART_HandleTypeDef UartHandle;

UINT32 g_uartrSemID;

static char uartfifo[2][1056];
static int curbuf = 0;

//#define LOS_UART_IT 1

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

static void MX_USART2_UART_Init(void)
{

  UartHandle.Instance = USART2;
  UartHandle.Init.BaudRate = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits = UART_STOPBITS_1;
  UartHandle.Init.Parity = UART_PARITY_NONE;
  UartHandle.Init.Mode = UART_MODE_TX_RX;
  UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

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
#ifdef LOS_STM32F103ZETx
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  LOS_SemCreate(0,&g_uartrSemID);
#endif
    
    return;
}


/*************************************************************************************************
 *  功能：向串口1发送一个字符                                                                    *
 *  参数：(1) 需要被发送的字符                                                                   *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartWriteByte(char c)
{
#ifdef LOS_STM32F103ZETx
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
	
#ifdef LOS_STM32F103ZETx
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
	
#ifdef LOS_STM32F103ZETx
    HAL_StatusTypeDef ret;

#ifdef LOS_UART_IT
    #if 0
			ret = HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)&str, len);
			if(ret != HAL_OK)
			{
				Error_Handler();
			}
    #else
			if(HAL_UART_Transmit(&UartHandle, (uint8_t*)str, len, 5000)!= HAL_OK)
			if(ret != HAL_OK)
			{
				Error_Handler();
			}
     #endif
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
#ifdef LOS_STM32F103ZETx
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
#ifdef LOS_STM32F103ZETx
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)uartfifo[0], len) != HAL_OK)
  {
    Error_Handler();
  }
#endif
	return ;
}

int LOS_EvbUartReadByte2(char* c, unsigned short len, unsigned int timeout)
{
    int rdlen = 0;
#ifdef LOS_STM32F103ZETx
    HAL_StatusTypeDef ret = HAL_OK;
#ifdef LOS_UART_IT
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)c, len) != HAL_OK)
  {
    Error_Handler();
  }
#else
  ret = HAL_UART_Receive(&UartHandle, (uint8_t *)c, len, timeout);
  if (ret == HAL_OK)
  {
      rdlen = UartHandle.RxXferSize - UartHandle.RxXferCount;
  }
  else if (ret == HAL_TIMEOUT)
  {
      rdlen = UartHandle.RxXferSize - UartHandle.RxXferCount - 1;
  }
  else
  {
      rdlen = 0;
  }
	//if(HAL_UART_Receive(&UartHandle, (uint8_t *)c, len, timeout) != HAL_OK)
	{
		//Error_Handler();  
	}
#endif
#endif
	return rdlen;
}


#ifdef LOS_STM32F103ZETx
static char _buffer[128];
#endif
void LOS_EvbUartPrintf(char* fmt, ...)
{
#ifdef LOS_STM32F103ZETx
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
    return;
}
