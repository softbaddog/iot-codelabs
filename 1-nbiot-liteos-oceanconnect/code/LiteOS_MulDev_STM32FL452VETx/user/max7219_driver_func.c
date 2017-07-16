
#include "los_bsp_led.h"
#include "los_task.h"

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_spi.h"
#include "max7219_driver_func.h"

#define SPIx_TIMEOUT_MAX              ((uint32_t)0x1000)
#define NUCLEO_MAX7219_CS_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()


SPI_HandleTypeDef SpiHandle;
uint32_t SpixTimeoutx = SPIx_TIMEOUT_MAX; /*<! Value of Timeout when SPI communication fails */  


unsigned char disp1[38][8]={
	{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0
	{0x10,0x18,0x14,0x10,0x10,0x10,0x10,0x10},//1
	{0x7E,0x2,0x2,0x7E,0x40,0x40,0x40,0x7E},//2
	{0x3E,0x2,0x2,0x3E,0x2,0x2,0x3E,0x0},//3
	{0x8,0x18,0x28,0x48,0xFE,0x8,0x8,0x8},//4
	{0x3C,0x20,0x20,0x3C,0x4,0x4,0x3C,0x0},//5
	{0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x0},//6
	{0x3E,0x22,0x4,0x8,0x8,0x8,0x8,0x8},//7
	{0x0,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E},//8
	{0x3E,0x22,0x22,0x3E,0x2,0x2,0x2,0x3E},//9
	{0x8,0x14,0x22,0x3E,0x22,0x22,0x22,0x22},//A
	{0x3C,0x22,0x22,0x3E,0x22,0x22,0x3C,0x0},//B
	{0x3C,0x40,0x40,0x40,0x40,0x40,0x3C,0x0},//C
	{0x7C,0x42,0x42,0x42,0x42,0x42,0x7C,0x0},//D
	{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C},//E
	{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40},//F
	{0x3C,0x40,0x40,0x40,0x40,0x44,0x44,0x3C},//G
	{0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44},//H
	{0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C},//I
	{0x3C,0x8,0x8,0x8,0x8,0x8,0x48,0x30},//J
	{0x0,0x24,0x28,0x30,0x20,0x30,0x28,0x24},//K
	{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C},//L
	{0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81},//M
	{0x0,0x42,0x62,0x52,0x4A,0x46,0x42,0x0},//N
	{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//O
	{0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20},//P
	{0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D},//Q
	{0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21},//R
	{0x0,0x1E,0x20,0x20,0x3E,0x2,0x2,0x3C},//S
	{0x0,0x3E,0x8,0x8,0x8,0x8,0x8,0x8},//T
	{0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C},//U
	{0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},//V
	{0x0,0x49,0x49,0x49,0x49,0x2A,0x1C,0x0},//W
	{0x0,0x41,0x22,0x14,0x8,0x14,0x22,0x41},//X
	{0x41,0x22,0x14,0x8,0x8,0x8,0x8,0x8},//Y
	{0x0,0x7F,0x2,0x4,0x8,0x10,0x20,0x7F},//Z
	{0x8,0x7F,0x49,0x49,0x7F,0x8,0x8,0x8},//ÖÐ
	{0xFE,0xBA,0x92,0xBA,0x92,0x9A,0xBA,0xFE},//¹ú
};



void LOS_MAX7219_Init_CS(void);
#if 0
static void SPIx_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable SPIx clock */
  SPIx_CLK_ENABLE();

  /* Enable DISCOVERY_SPI GPIO clock */
  SPIx_SCK_GPIO_CLK_ENABLE();

  /* configure SPI SCK, MOSI and MISO */    
  GPIO_InitStructure.Pin    = SPIx_SCK_PIN|SPIx_MISO_PIN|SPIx_MOSI_PIN;//(DISCOVERY_SPIx_SCK_PIN | DISCOVERY_SPIx_MOSI_PIN | DISCOVERY_SPIx_MISO_PIN);
  GPIO_InitStructure.Mode   = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull   = GPIO_NOPULL;
  GPIO_InitStructure.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPIx_GPIO_PORT, &GPIO_InitStructure);    

}
#endif
/* SPI1 init function */
void MX_SPI1_Init(void)
{
  SpiHandle.Instance = SPI1;
  SpiHandle.Init.Mode = SPI_MODE_MASTER;
  SpiHandle.Init.Direction = SPI_DIRECTION_2LINES;
  SpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
  SpiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
  SpiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
  SpiHandle.Init.NSS = SPI_NSS_SOFT;
  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SpiHandle.Init.TIMode = SPI_TIMODE_DISABLE;
  SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SpiHandle.Init.CRCPolynomial = 7;
  //SpiHandle.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  //SpiHandle.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&SpiHandle) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/**
  * @brief  SPIx Bus initialization
  */
void LOS_MAX7219_Driver_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  if(HAL_SPI_GetState(&SpiHandle) == HAL_SPI_STATE_RESET)
  {
   #if 0
    /* SPI configuration -----------------------------------------------------*/
    SpiHandle.Instance = SPIx;
    /* SPI baudrate is set to 5.6 MHz (PCLK2/SPI_BaudRatePrescaler = 90/16 = 5.625 MHz) 
       to verify these constraints:
       - ILI9341 LCD SPI interface max baudrate is 10MHz for write and 6.66MHz for read
       - l3gd20 SPI interface max baudrate is 10MHz for write/read
       - PCLK2 frequency is set to 90 MHz 
    */  
    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;

    /* On STM32F429I-Discovery, LCD ID cannot be read then keep a common configuration */
    /* for LCD and GYRO (SPI_DIRECTION_2LINES) */
    /* Note: To read a register a LCD, SPI_DIRECTION_1LINE should be set */
    SpiHandle.Init.Direction      = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase       = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity    = SPI_POLARITY_LOW;
    SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    SpiHandle.Init.CRCPolynomial  = 7;
    SpiHandle.Init.DataSize       = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit       = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS            = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode         = SPI_TIMODE_DISABLED;
    SpiHandle.Init.Mode           = SPI_MODE_MASTER;
  
    SPIx_MspInit(&SpiHandle);
    HAL_SPI_Init(&SpiHandle);
    #else
    MX_SPI1_Init();
    #endif
	LOS_MAX7219_Init_CS();
  } 
}

static void LOS_SPIx_Error(void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&SpiHandle);
  __HAL_RCC_GPIOA_CLK_DISABLE();
  /* Re- Initialize the SPI communication BUS */
  LOS_MAX7219_Driver_Init();
}

/**
  * @brief  Reads 4 bytes from device.
  * @param  ReadSize: Number of bytes to read (max 4 bytes)
  * @retval Value read on the SPI
  */
uint32_t LOS_SPIx_Read(uint8_t ReadSize)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t readvalue;
  
  status = HAL_SPI_Receive(&SpiHandle, (uint8_t*) &readvalue, ReadSize, SpixTimeoutx);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initialize the BUS */
    LOS_SPIx_Error();
  }
  
  return readvalue;
}

/**
  * @brief  Writes a byte to device.
  * @param  Value: value to be written
  */
void LOS_SPIx_Write(uint16_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_SPI_Transmit(&SpiHandle, (uint8_t*) &Value, 1, SpixTimeoutx);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initialize the BUS */
    LOS_SPIx_Error();
  }
}

void LOS_MAX7219_Init_CS(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
  NUCLEO_MAX7219_CS_GPIO_CLK_ENABLE();
  
  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); 
}

void LOS_MAX7219_Ctrl_CS(int high)
{
	if (high)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); 
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); 
	}
	return ;
}

void LOS_MAX7219_Write(unsigned char address, unsigned char value)
{
	LOS_MAX7219_Ctrl_CS(0);
	LOS_SPIx_Write(address);
	LOS_SPIx_Write(value);
	LOS_MAX7219_Ctrl_CS(1);
}

void LOS_MAX7219_Init(void)
{
	LOS_MAX7219_Driver_Init();
	LOS_MAX7219_Write(0x09, 0x00);       //set BCD code as decode mode
	LOS_MAX7219_Write(0x0a, 0x03);       //set brightness
	LOS_MAX7219_Write(0x0b, 0x07);       //set scan bounary 8 Digital tube
	LOS_MAX7219_Write(0x0c, 0x01);       //set mode£º0 power lost mode, 1 normal mode 
	LOS_MAX7219_Write(0x0f, 0x00);       //show test 1, test finish and ok 0
}

void LOS_MAX7219_show(char data)
{
	int j = 0;
	int i = 0;
	if ('0' <= data && '9' >= data)
	{
		j = data - 0x30;
	}
	if ('A' <= data && 'Z'>= data)
	{
		j = data - 0x41 + 10;
	}
	if ('a' <= data && 'z'>= data)
	{
		j = data - 0x61 + 10;
	}
	for(i=1;i<9;i++)
	{
		LOS_MAX7219_Write(i,disp1[j][i-1]);
	}
	//Delay_xms(1000);
}







