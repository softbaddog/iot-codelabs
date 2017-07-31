#ifndef __BIT_BAND_H__
#define __BIT_BAND_H__

#include "bit_band.h"
#include "cmsis_os.h"

#define LED0_IN()  {GPIOB->MODER&=~(3<<(15*2));GPIOB->MODER|=0<<15*2;}
#define LED0_OUT() {GPIOB->MODER&=~(3<<(15*2));GPIOB->MODER|=1<<15*2;}

#define LED1_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}
#define LED1_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;}

#define LED0    PBout(15)
#define LED1    PBout(7)

static void MX_GPIO_Init(void);

void led_test(void);

#endif
