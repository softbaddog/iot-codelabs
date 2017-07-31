#include "uart_test.h"

extern double TemRet;
extern double HumRet;

volatile uint8_t InitImeiFlag = 0;
char ImeiNumber[15];

void FT_UART_Send(void)
{
	los_dev_uart_write(LOS_STM32L476_UART3, "Welcome to use ThunderSoft NB-IoT develop board!\n", 49, 100);	
	osDelay(1000);
	los_dev_uart_write(LOS_STM32L476_UART3, "欢迎使用中科创达NB-IoT开发板！\n", 43, 100);
	osDelay(1000);

	while(1)
	{
		//循环发送温湿度和NB-IoT模块IMEI值
		char send_buff[1024];
		sprintf(send_buff, "01.Temperature is %f, Humidity is %f\n", TemRet, HumRet);
		los_dev_uart_write( LOS_STM32L476_UART3, send_buff, strlen(send_buff), 1000);
		osDelay(30000);
		memset(send_buff, 0, sizeof(send_buff));
		if(InitImeiFlag)
		{
			sprintf(send_buff, "02.NB-Iot module IMEI : %s\n", ImeiNumber);
			los_dev_uart_write( LOS_STM32L476_UART3, send_buff, strlen(send_buff), 1000);
			osDelay(1000);
			memset(send_buff, 0, sizeof(send_buff));
		}
	}
}

void NB_TEST_Uart(void)
{
	char read_buff[64];
	char *tmp = NULL;

	/*获取NB-IoT模块的IMEI号*/
	if(!InitImeiFlag)
	{
		los_dev_uart_write(LOS_STM32L476_UART2, "AT+CGSN=1\r\n", 11, 100);
		los_dev_uart_read(LOS_STM32L476_UART2, read_buff, 64, 5000);
		osDelay(1000);
			
		tmp = strstr(read_buff, "CGSN");
		if(tmp!=NULL)
		{
			ImeiNumber[0] = read_buff[8];
			ImeiNumber[1] = read_buff[9];
			ImeiNumber[2] = read_buff[10];
			ImeiNumber[3] = read_buff[11];
			ImeiNumber[4] = read_buff[12];
			ImeiNumber[5] = read_buff[13];
			ImeiNumber[6] = read_buff[14];
			ImeiNumber[7] = read_buff[15];
			ImeiNumber[8] = read_buff[16];
			ImeiNumber[9] = read_buff[17];
			ImeiNumber[10] = read_buff[18];
			ImeiNumber[11] = read_buff[19];
			ImeiNumber[12] = read_buff[20];
			ImeiNumber[13] = read_buff[21];
			ImeiNumber[14] = read_buff[22];
			InitImeiFlag = 1;
		}
	}
}
