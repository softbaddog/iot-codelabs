#include "stdint.h"
#include "stm32l4xx.h"
#include "delay.h"

#define SYSTEM_SUPPORT_OS 1

uint16_t fac_us;
uint16_t fac_ms;

void delay_init()
{
	/*SysTick->CTRL&=0xFFFFFFFB;
	fac_us = 216/8;
	fac_ms = (uint16_t)fac_us*1000;*/
#if 0
#if SYSTEM_SUPPORT_OS //?????? OS.
  uint32_t reload;
#endif
    
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	//SysTick ??? HCLK
	fac_us=80000; //?????? OS,fac_us ?????
    
#if SYSTEM_SUPPORT_OS //?????? OS.
	reload=80000; //???????? ??? K
	reload*=1000000/delay_ostickspersec; //?? delay_ostickspersec ??????
	//reload ? 24 ????,???:16777216,? 180M ?,?? 0.745s ??
	fac_ms=1000/delay_ostickspersec; //?? OS ?????????
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//?? SYSTICK ??
	SysTick->LOAD=reload; //? 1/OS_TICKS_PER_SEC ?????
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //?? SYSTICK
#else
#endif
#endif
}

void delay_us(uint32_t nus)
{
	/*uint32_t temp;
	SysTick->LOAD=nus*fac_us;
	SysTick->VAL=0x00;  
  SysTick->CTRL=0x01;  
  do  
  {  
      temp=SysTick->CTRL;  
  }  
  while(temp&0x01&&!(temp&0x10000));  
  SysTick->CTRL=0x00;  
  SysTick->VAL=0x00;*/
#if 0
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD; //LOAD ??
	ticks=nus*fac_us; //??????
	told=SysTick->VAL; //?????????
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)tcnt+=told-tnow;//???? SYSTICK ??????????.
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break; //????/????????,???.
		}
  };
#endif
#if 1
	int i;
	for(i=0; i<10*nus; i++){}
#endif
#if 0
	SysTick->LOAD=80*nus;
	SysTick->CTRL=0x00000005;
	while(!(SysTick->CTRL&0x00010000));
	SysTick->CTRL=0x00000004;
#endif
}

void delay_ms(uint16_t nms)  
{  
  uint32_t temp;  
  SysTick->LOAD=(uint32_t)(nms*fac_ms);  
  SysTick->VAL=0x00;  
  SysTick->CTRL=0x01;  
  do  
  {  
      temp=SysTick->CTRL;  
  }  
  while(temp&0x01&&!(temp&0x10000));  
  SysTick->CTRL=0x00;  
  SysTick->VAL=0x00;     
}