#include "los_sys.h"
#include "los_tick.h"
#include "los_task.ph"
#include "los_config.h"

#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"
#include "cmsis_os.h"

#include <string.h>

#include "stm32l4xx_hal.h"

#include "los_dev_st_uart.h"
#include "los_dev_st_spi.h"
#include "udp_coap_interface.h"

#include "uart_test.h"
#include "th_sensor.h"
#include "gpio_handle.h"

extern void LOS_EvbSetup(void);

extern void oceancon_sampletask(void * pvParameters);

static char debug_uart[500];
static char nbiot_uart[500];
double HumRet=0;
double TemRet=0;

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
    ret = los_dev_uart_init(LOS_STM32L476_UART3, 9600, tb2, 500);
    if (ret != 0)
    {
        while(1);
    }
		while(1)
		{
        //los_dev_uart_write(LOS_STM32L476_UART3, "123\n", 4, 100);
        //len = uart_data_read(tb1, 500, 0);
        len = los_dev_uart_read(LOS_STM32L476_UART3, tb1, 4, 5000); 
        if (len > 0)
            los_dev_uart_write(LOS_STM32L476_UART3, tb1, len, 100);
				osDelay(10);
		}
}
unsigned char testlip[4] = {192, 168, 206, 41};
unsigned char testlgw[4] = {192, 168, 206, 1};
unsigned char testlmsk[4] = {255, 255, 255, 0};
void init_testethernet(void * pvParameters)
{
    //int len;
    int ret = 0;
    unsigned char testip[4] = {192, 168, 206, 39};
    //unsigned char testip[4] = {112,93,129,154};
    
    ret = los_bus_spi_init(LOS_STM32L476_SPI1);
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
			udp_send_data(ret, "12345", 5);
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
extern void KEY_init(void);
LITE_OS_SEC_TEXT_INIT
int main(void)
{
	UINT32 uwRet;
	
	/*声明6个线程，分别用于串口输出，NB-IoT测试，温湿度传感器，LED灯，气压传感器以及六轴传感器*/
	osThreadDef_t debug_thread;
	osThreadDef_t nbiot_thread;
	osThreadDef_t thsensor_thread;
	osThreadDef_t led_thread;
  //osThreadDef_t oceanconnect_thread;
	
	UINT32 iRet;
	
  /*初始化LiteOS的内核*/
  uwRet = LOS_KernelInit();
  if (uwRet != LOS_OK)
	{
      return LOS_NOK; 
  }

  HAL_Init();

  /*配置系统时钟*/
  SystemClock_Config();
    
  /*使能LiteOS系统计时器中断*/
  LOS_EnableTick();
 
	/*初始化调试串口，在系统启动后会通过串口打印NB-IoT模块的IMEI号和传感器的原始数据*/
	iRet = los_dev_uart_init(LOS_STM32L476_UART3, 9600, debug_uart, 500);
	if( iRet!=0 )
	{
		osDelay(1000);
	}
	
	/*初始化NB-IoT串口，在系统启动后通过该串口对NB-IoT进行调试*/
	iRet = los_dev_uart_init(LOS_STM32L476_UART2, 9600, nbiot_uart, 500);
	if( iRet!=0 )
	{
		osDelay(1000);
	}
	
	/*初始化硬件I2C，该I2C用于获取温湿度传感器的数据*/
	th_iic_Init();
	
	/*初始化按键，该按键用于控制LED灯的状态*/
	KEY_init();
	
	/*设置线程*/
	debug_thread.name = "Test_UART";
	debug_thread.stacksize = 2048;
	debug_thread.tpriority = osPriorityLow;
	debug_thread.pthread = (os_pthread)FT_UART_Send;                   //串口测试函数，debug_thread线程执行该函数
	osThreadCreate(&debug_thread, NULL);
	
	nbiot_thread.name = "Test_NB";
	nbiot_thread.stacksize = 2048;
	nbiot_thread.tpriority = osPriorityLow;
	nbiot_thread.pthread = (os_pthread)NB_TEST_Uart;                   //NB-IoT测试函数，nbiot_thread线程执行该函数
	osThreadCreate(&nbiot_thread, NULL);
  
	thsensor_thread.name = "TH_Sensor";
	thsensor_thread.stacksize = 2048;
	thsensor_thread.tpriority = osPriorityLow;
	thsensor_thread.pthread = (os_pthread)SHT20_test;                  //温湿度测试函数
	osThreadCreate(&thsensor_thread, NULL);
	
	led_thread.name = "Test_LED";
	led_thread.stacksize = 2048;
	led_thread.tpriority = osPriorityLow;
	led_thread.pthread = (os_pthread)led_test;                         //LED灯闪烁函数
	osThreadCreate(&led_thread, NULL);
	
	//oceanconnect_thread.name = "OceanCon_Sample";										// OceanConnect对接测试
	//oceanconnect_thread.stacksize = 2048;
	//oceanconnect_thread.tpriority = osPriorityHigh;
	//oceanconnect_thread.pthread = (os_pthread)oceancon_sampletask;
	//osThreadCreate(&oceanconnect_thread, NULL);

  /* Kernel start to run */
  LOS_Start();
  for (;;);
}
