#ifndef __DELAY_H__
#define __DELAY_H__

#include "stdint.h"

//uint16_t fac_us;
//uint16_t fac_ms;

void delay_init();
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
#endif