#include "los_bsp_uart.h"
#include <stdarg.h>

#ifdef LOS_STM32L452VE


#endif



void LOS_EvbUartInit(void)
{

    return ;
}


/*************************************************************************************************
 *  功能：向串口1发送一个字符                                                                    *
 *  参数：(1) 需要被发送的字符                                                                   *
 *  返回：                                                                                       *
 *  说明：                                                                                       *
 *************************************************************************************************/
void LOS_EvbUartWriteByte(char c)
{
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

    return ;
}

void LOS_EvbUartWriteStr2(const char* str, unsigned short len)
{
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
    return ;
}

int LOS_EvbUartReadByte2(char* c, unsigned short len, unsigned int timeout)
{
	return 0;
}


#ifdef LOS_STM32L452VE
static char _buffer[128];
#endif
void LOS_EvbUartPrintf(char* fmt, ...)
{
#ifdef LOS_STM32L452VE
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
    return ;
}
