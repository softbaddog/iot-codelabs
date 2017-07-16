#include "los_sys.h"
#include "los_tick.h"
#include "los_task.ph"
#include "los_config.h"

#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"
//#include "los_inspect_entry.h"
//#include "los_demo_entry.h"
#include "cmsis_os.h"

#include <string.h>

#include "stm32l4xx_hal.h"

#include "los_dev_st_uart.h"
#include "los_dev_st_spi.h"
#include "udp_coap_interface.h"

extern void LOS_EvbSetup(void);


void SystemClock_Config(void);
void _Error_Handler(char * file, int line);

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
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

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}
#endif

static UINT32 g_uwboadTaskID;
LITE_OS_SEC_TEXT VOID LOS_BoadExampleTskfunc(VOID)
{
    while (1)
    {
        //LOS_EvbLedControl(LOS_LED2, LED_ON);
        LOS_EvbUartWriteStr("Board Test\n");
        LOS_TaskDelay(500);
        //LOS_EvbLedControl(LOS_LED2, LED_OFF);
        LOS_TaskDelay(500);
    }
}
void LOS_BoadExampleEntry(void)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_BoadExampleTskfunc;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTaskInitParam.pcName = "BoardDemo";
    stTaskInitParam.usTaskPrio = 10;
    uwRet = LOS_TaskCreate(&g_uwboadTaskID, &stTaskInitParam);

    if (uwRet != LOS_OK)
    {
        return;
    }
    return;
}
static char tb2[500];
static char tb1[500];
void init_testuart(void * pvParameters)
{
    int len;
    int ret = 0;
    ret = los_dev_uart_init(LOS_STM32L452_UART3, 9600, tb2, 500);
    if (ret != 0)
    {
        while(1);
    }
	while(1)
	{
        //los_dev_uart_write(LOS_STM32L452_UART3, "123\n", 4, 100);
        //len = uart_data_read(tb1, 500, 0);
        len = los_dev_uart_read(LOS_STM32L452_UART3, tb1, 4, 5000); 
        if (len > 0)
            los_dev_uart_write(LOS_STM32L452_UART3, tb1, len, 100);
		osDelay(10);
	}
}
unsigned char testlip[4] = {192, 168, 3, 41};
unsigned char testlgw[4] = {192, 168, 3, 1};
unsigned char testlmsk[4] = {255, 255, 255, 0};
void init_testethernet(void * pvParameters)
{
    int len;
    int ret = 0;
    unsigned char testip[4] = {192, 168, 206, 39};
    //unsigned char testip[4] = {112,93,129,154};
    
    ret = los_bus_spi_init(LOS_STM32L452_SPI1);
    ret = udp_set_local_addr(testlip, testlgw, testlmsk);
    ret = udp_stack_init();
    
    if (ret != 0)
    {
        while(1);
    }
    osDelay(10);
    ret = udp_create_socket(testip, 5683);
    osDelay(1000);
	while(1)
	{
        //len = uart_data_read(tb1, 500, 0);
        len = udp_send_data(ret, "12345", 5);
		osDelay(1000);
	}
}

/*****************************************************************************
Function    : main
Description : Main function entry
Input       : None
Output      : None
Return      : None
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT
int main(void)
{
	UINT32 uwRet;
	osThreadDef_t thread_def;
    /*
        add you hardware init code here
        for example flash, i2c , system clock ....
     */
    /*Init LiteOS kernel */
    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK) {
        return LOS_NOK; 
    }
    //HAL_init();....
    HAL_Init();

    /* Configure the system clock to 216 MHz */
    SystemClock_Config();
    
    /* Enable LiteOS system tick interrupt */
    LOS_EnableTick();

    /*
        Notice: add your code here
        here you can create task for your function 
        do some hw init that need after systemtick init
     */
    LOS_EvbSetup();//init the device on the dev baord
    
    
    //LOS_Demo_Entry();

    //LOS_Inspect_Entry();

    //LOS_BoadExampleEntry();

    thread_def.name = "Test";
	thread_def.stacksize = 1900;
	thread_def.tpriority = osPriorityLow;
	thread_def.pthread = (os_pthread)init_testuart;
	//osThreadCreate(&thread_def, NULL);

    thread_def.name = "Test";
	thread_def.stacksize = 1900;
	thread_def.tpriority = osPriorityLow;
	thread_def.pthread = (os_pthread)init_testethernet;
	//osThreadCreate(&thread_def, NULL);
    
    los_oceancon_sample();
    

    /* Kernel start to run */
    LOS_Start();
    for (;;);
    /* Replace the dots (...) with your own code. */
}
