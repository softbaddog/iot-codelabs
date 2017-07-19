#include "los_bsp_led.h"

#ifdef LOS_STM32L476xx

#include "stm32l4xx_hal.h"


#endif

void LOS_EvbLedInit(void)
{

#ifdef LOS_STM32L476xx

#endif

    return;
}

/*************************************************************************************************
 *  function£ºcontrol led on off                                                                 *
 *  param (1) index Led's index                                                                  *
 *        (2) cmd   Led on or off                                                                *
 *  return : None                                                                                *
 *  discription:                                                                                 *
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_STM32L476xx
    switch (index)
    {
        case LOS_LED2:
        {
            if (cmd == LED_ON)
            {
                 /*led2 on */
            }
            else
            {
                /*led2 off */
            }
            break;
        }
        default:
        {
            break;
        }
    }
#endif
    return;
}


