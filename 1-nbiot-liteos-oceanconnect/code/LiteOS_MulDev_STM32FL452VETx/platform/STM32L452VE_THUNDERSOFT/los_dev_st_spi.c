#include "los_dev_st_spi.h"
#include "stm32l4xx_hal_spi.h"
#include "stm32l4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


SPI_TypeDef *spibus[] = {
    SPI1,
    SPI2
};

#define LOS_BUS_SPI_NUM 2
los_bus_spi_t spibuses[LOS_BUS_SPI_NUM];

static void los_dev_spi_gpio_init(int spiidx)
{

    /* GPIO Ports Clock Enable */
    if (spiidx == LOS_STM32L452_SPI1)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    if (spiidx == LOS_STM32L452_SPI2)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    return ;
}

int los_bus_spi_init(int spiidx)
{
    if (spiidx <= 0 || spiidx > LOS_BUS_SPI_NUM)
    {
        return -1;
    }
    los_dev_spi_gpio_init(spiidx);
    /* init read write mutex and lock it */
    //LOS_SemCreate(0,&spibuses[spiidx -1].rsem);
    //LOS_SemCreate(1,&spibuses[spiidx -1].wsem);
    
    //Note that here spi bus just used for enc28j60, so we init spi1 as follows param
    
    spibuses[spiidx].dev.Instance = spibus[spiidx - 1];
    spibuses[spiidx].dev.Init.Mode = SPI_MODE_MASTER;
    spibuses[spiidx].dev.Init.Direction = SPI_DIRECTION_2LINES;
    spibuses[spiidx].dev.Init.DataSize = SPI_DATASIZE_8BIT;
    spibuses[spiidx].dev.Init.CLKPolarity = SPI_POLARITY_LOW;
    spibuses[spiidx].dev.Init.CLKPhase = SPI_PHASE_1EDGE;
    spibuses[spiidx].dev.Init.NSS = SPI_NSS_SOFT;
    if (spiidx == LOS_STM32L452_SPI1)
    {
        // here we need to change it to a new value
        spibuses[spiidx].dev.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    }
    if (spiidx == LOS_STM32L452_SPI2)
    {
        // spi2 bus pins not drawed out, so it just a test code here
        spibuses[spiidx].dev.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    }
    spibuses[spiidx].dev.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spibuses[spiidx].dev.Init.TIMode = SPI_TIMODE_DISABLE;
    spibuses[spiidx].dev.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    spibuses[spiidx].dev.Init.CRCPolynomial = 7;
    spibuses[spiidx].dev.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    spibuses[spiidx].dev.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
    if (HAL_SPI_Init(&spibuses[spiidx].dev) != HAL_OK)
    {
        while(1);
    }
    
//    spibuses[spiidx].pbuf = buf;
//    spibuses[spiidx].dualbuf = buf + size / 2;
//    spibuses[spiidx].interrbuf = NULL;
//    spibuses[spiidx].size = size;
//    spibuses[spiidx].readcnt = 0;
//   
    return 0;
}
//if(HAL_SPI_Transmit(&spibuses[spiidx].dev, outbuf, rsize, mstimout) != HAL_OK)

int los_bus_spi_read(int spiidx, char *outbuf, int rsize, int mstimout)
{
    if(HAL_SPI_Receive(&spibuses[spiidx].dev, (uint8_t *)outbuf, rsize, mstimout) != HAL_OK)
    {
        return -1;
    }
    return spibuses[spiidx].dev.RxXferSize - spibuses[spiidx].dev.RxXferCount;
}

int los_bus_spi_write(int spiidx, char *inbuf, int wsize, int mstimout)
{
    if(HAL_SPI_Transmit(&spibuses[spiidx].dev, (uint8_t *)inbuf, wsize, mstimout) != HAL_OK)
    {
        return -1;
    }
    return 0;
}

unsigned char los_bus_spi_rwonebyte(int spiidx, unsigned char writedat, int mstimout)
{
    unsigned char out = 0x0;
    if(HAL_SPI_TransmitReceive(&spibuses[spiidx].dev, (uint8_t *)&writedat, (uint8_t *)&out, 1, mstimout)!= HAL_OK)
    {
        return out;
    }
    return out;
}

int los_bus_spi_devcs_init(GPIO_TypeDef  *GPIOx, unsigned short pin_num)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable the GPIO_LED Clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure the GPIO_LED pin */
    GPIO_InitStruct.Pin = pin_num;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
    
    return 0;
}

