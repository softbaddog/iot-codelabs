#include "los_bsp_key.h"

#ifdef LOS_STM32L452VE

#endif

void LOS_EvbKeyInit(void)
{

#ifdef LOS_STM32L452VE

#endif

    return;
}

#ifdef LOS_STM32L452VE
/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)

#endif

/*****************************************************************************
 Function    : LOS_EvbGetKeyVal
 Description : Get GIOP Key value
 Input       : int KeyNum
 Output      : None
 Return      : KeyVal
 *****************************************************************************/
unsigned int LOS_EvbGetKeyVal(int KeyNum)
{
    unsigned int KeyVal = LOS_GPIO_ERR; 

    //add you code here.

    return KeyVal;
}

