#ifndef _LOS_DEV_ST_SPI_H
#define _LOS_DEV_ST_SPI_H


//LiteOS header files

#include "los_base.ph"
#include "los_hwi.h"
#include "los_sem.h"

//BSP header files
#include "stm32f1xx_hal.h"

#define LOS_STM32F103_SPI1 1
#define LOS_STM32F103_SPI2 2

typedef struct _los_bus_spi_t
{
    SPI_HandleTypeDef dev;
    //UINT32 rsem;// liteos mutex for uart read
    //UINT32 wsem;// liteos mutex for uart write
    //char *pbuf;//   uart receive data buf
    //char *dualbuf;//   uart receive data buf
    //char *interrbuf;//   uart receive data buf
    //int size;  //   uart receive data buf size
    //int readcnt;//  received data bytes
}los_bus_spi_t;


int los_bus_spi_init(int spiidx);

int los_bus_spi_read(int spiidx, char *outbuf, int rsize, int mstimout);

int los_bus_spi_write(int spiidx, char *inbuf, int wsize, int mstimout);

unsigned char los_bus_spi_rwonebyte(int spiidx, unsigned char writedat, int mstimout);

int los_bus_spi_devcs_init(GPIO_TypeDef  *GPIOx, unsigned short pin_num);

void los_bus_spi_irqhandler(int spiidx);

#endif
