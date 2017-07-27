#include "th_sensor.h"

uint16_t  POLYNOMIAL = 0x131;
I2C_HandleTypeDef th_iic;

void _IIC_Error_Handler(char *File, int Line)
{
	osDelay(1000);
}

static void th_iic_gpio_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

void th_iic_Init(void)
{
	th_iic_gpio_Init();
	
  th_iic.Instance = I2C1;
  //th_iic.Init.Timing = 0x10909CEC;
	th_iic.Init.Timing = 0x00000004;
  th_iic.Init.OwnAddress1 = 0;
  th_iic.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  th_iic.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  th_iic.Init.OwnAddress2 = 0;
  th_iic.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  th_iic.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  th_iic.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&th_iic) != HAL_OK)
  {
    _IIC_Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&th_iic, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _IIC_Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&th_iic, 0) != HAL_OK)
  {
    _IIC_Error_Handler(__FILE__, __LINE__);
  }

}

uint8_t i2c_reg_write( uint8_t device_addr,uint8_t reg, uint8_t value )
{
	uint8_t status = HAL_OK;
  uint8_t TxData[2] = {0};
  TxData[0]= reg;
  TxData[1]= value;

  status = HAL_I2C_Master_Transmit(&th_iic, device_addr, TxData, 2, 1000);
  if(status != HAL_OK) {}
  return status;
}

uint8_t i2c_reg_read( uint8_t device_addr,uint8_t reg, uint8_t *value )
{
  uint8_t status1 = HAL_OK,status2 = HAL_OK;
  static uint8_t  msg[1];
  uint8_t value_read[2];
  msg[0] = reg;

  status1 = HAL_I2C_Master_Transmit(&th_iic, device_addr, msg, 1, 1000);
  status2 = HAL_I2C_Master_Receive(&th_iic, device_addr, value_read, 1, 1000);

  *value = value_read[0];
  // *value = ( value_read[0] << 8 ) + value_read[1];
  if((status1 != HAL_OK)||(status2 != HAL_OK)) {}
  return (status1|status2);
}

uint8_t i2c_reg_read_len( uint8_t device_addr,uint8_t reg, uint32_t *value ,uint8_t len)  //len <=  3
{
  uint8_t status1 = HAL_OK,status2 = HAL_OK;
  static uint8_t  msg[1];
  uint8_t value_read[3];
  msg[0] = reg;

  status1 = HAL_I2C_Master_Transmit(&th_iic, device_addr, msg, 1, 1000);
  status2 = HAL_I2C_Master_Receive(&th_iic, device_addr, value_read, len, 1000);

  *value = ( value_read[0] << 16 ) + (value_read[1]<< 8 ) + value_read[2];
  if((status1 != HAL_OK)||(status2 != HAL_OK)) {}
  return (status1|status2);
}

uint8_t i2c_cmd_send( uint8_t device_addr,uint8_t cmd)
{
  uint8_t status = HAL_OK;
  uint8_t TxData[1] = {0};
  TxData[0]= cmd;
  status = HAL_I2C_Master_Transmit(&th_iic, device_addr, TxData, 1, 1000);
  if(status != HAL_OK) {}
  return status;
}



uint8_t SHT20_crc_check(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum)
{
  uint8_t crc = 0;
  uint8_t byteCtr;
  for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
  {   crc ^= (data[byteCtr]);
      for (uint8_t bit = 8; bit > 0; --bit)
      {   if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
          else crc = (crc << 1);
      }
  }
  if (crc != checksum)
      return 1;
  else return 0;
}
void SHT20_rest(void)
{
  uint8_t status = HAL_OK;
  status = i2c_cmd_send( SHT20_ADDRESS,SHT20_SOFT_REST);
  HAL_Delay(5);
  if(status != HAL_OK) {}
}

uint8_t SHT20_user_reg_read(uint8_t *RegValue)
{
  uint8_t checksum;
  uint8_t error=0;

  i2c_reg_read( SHT20_ADDRESS,SHT20_REG_READ, RegValue );
  error |= SHT20_crc_check (RegValue,1,checksum);
  return error;
}

uint8_t SHT20_user_reg_write(uint8_t RegValue)
{
  return 	i2c_reg_write( SHT20_ADDRESS,SHT20_REG_WRITE,RegValue );
}

uint16_t SHT20_measure_HM(SHT20_Measure_dat Mtype,uint16_t *RegValue)
{
  uint8_t checksum;
  uint8_t data[2]= {0};
  uint8_t error=0;
  uint32_t temp[1] = {0};

  switch(Mtype)
  {
  case HUMIDITY:
      i2c_reg_read_len( SHT20_ADDRESS,M_SHT20_READ_RH,temp,3);
      break;
  case TEMP :
      i2c_reg_read_len( SHT20_ADDRESS,M_SHT20_READ_T,temp,3);
      break;
  default:
      break;
  }

  *RegValue = (uint16_t)( temp[0] >> 8 );

  data[0] = (uint8_t)( temp[0] >> 16 );
  data[1] = (uint8_t)( temp[0] >> 8  );
  checksum = (uint8_t)temp[0];

  error |= SHT20_crc_check (data,2,checksum);

  return error;
		//return 0;
}

float SHT20_humidityRH_cal(uint16_t dat)
{
  float humidityRH;
  dat &= ~0x0003;

  humidityRH = -6.0 + 125.0/65536 * (float)dat;
  return humidityRH;
}

float SHT20_temperatureC_cal(uint16_t dat)
{
  float temperatureC;
  dat &= ~0x0003;
  temperatureC= -46.85 + 175.72/65536 *(float)dat;
  return temperatureC;
}

extern double TemRet;
extern double HumRet;

void SHT20_test(void)
{
	uint16_t data[2] = {0};
  uint16_t hum = 0;

	SHT20_rest();
	osDelay(1000);

	while(1)
	{
		if(SHT20_measure_HM(HUMIDITY,data) == 0 )
		{
			hum = data[0];
      HumRet = SHT20_humidityRH_cal(hum);
		}

		HAL_Delay(50);
		if( SHT20_measure_HM(TEMP,data) == 0)
		{
			hum = data[0];
      TemRet = SHT20_temperatureC_cal(hum);
		}

		osDelay(1000);
	}
}
