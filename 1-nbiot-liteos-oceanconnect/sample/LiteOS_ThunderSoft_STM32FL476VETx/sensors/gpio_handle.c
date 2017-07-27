#include "gpio_handle.h"
#include "stm32l4xx_hal.h"
#include "delay.h"

static void MX_GPIO_LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB14 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void led_test()
{
	MX_GPIO_LED_Init();
	osDelay(1000);
	LED0_OUT();
	osDelay(500);
	LED1_OUT();
	osDelay(500);
	while(1)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
		osDelay(1000);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
		osDelay(1000);
	}
}