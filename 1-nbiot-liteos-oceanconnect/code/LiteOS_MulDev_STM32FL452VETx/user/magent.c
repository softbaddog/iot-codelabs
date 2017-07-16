#include "magent.h"
#include "stm32f1xx_hal.h"

unsigned int g_uwMailCount = 0;
unsigned int g_uwMailOpen = 0;

void dev_magentsenor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  //__HAL_RCC_GPIOC_CLK_ENABLE();
  //__HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  /*Configure GPIO pin : PG7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PG8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
 /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);


	return ;
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static char count = 0;
    static char count2 = 0;
    if (GPIO_PIN_7 == GPIO_Pin)
    {  
        
        count++;
        if (count == 7)
        {
            g_uwMailCount++;
            count = 0;
        }
    }
	else if(GPIO_PIN_8 == GPIO_Pin)
	{
        count2++;
        if (count2 == 7)
        {
            if(g_uwMailCount > 0)
            {
                g_uwMailCount = 0;  
            }
            count2 = 0;
            g_uwMailOpen = 1;
        }
	}
}

unsigned int dev_get_mailcount(void)
{
	return g_uwMailCount;
}
unsigned int dev_get_mailopen(void)
{
	return g_uwMailOpen;
}

int dev_clear_mailopen()
{
    g_uwMailOpen = 0;
    return 0;
}
