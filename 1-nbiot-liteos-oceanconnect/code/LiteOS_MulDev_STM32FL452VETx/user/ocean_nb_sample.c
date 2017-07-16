//#include "ocean_sample.h"
#include "los_bsp_led.h"
#include "los_task.h"
#include "cmsis_os.h"
#include "max7219_driver_func.h"
#include "neul_bc95.h"
#include "los_dev_st_uart.h"

static int nb_get_cmd;
static int nb_uset_value;

#define LOS_DEV_UART_DUALBUF_SIZE 1024
char los_uart_dualbuf[LOS_DEV_UART_DUALBUF_SIZE] = {0};

char max7219_nbget_show_data(int idx)
{
	if (idx < 0  || idx > 38)
	{
		return '0';
	}
	if (0<=idx && 9>= idx)
	{
		return 0x30+idx;
	}
	if (10<=idx || 35>=idx)
	{
		return 0x41 + idx - 10;
	}
	return '0';
}




void oceancon_nb_task(void * pvParameters)
{
	unsigned char data[8] = {0};
	//int leds;
	int testd = 0;
	char showd;
    int ret = 0;

    //init nb chips
    ret = neul_bc95_hw_init();
    if (!ret)
    {
        //init bc 95 ok
        ret = neul_bc95_get_netstat();
        while(ret != 0)
        {
            //get network attacted flag
            ret = neul_bc95_get_netstat();
            if (0 == ret)
            {
                ret = neul_bc95_query_ip();
            }
            osDelay(1000);
        }
    }
    else
    {
        //init failed
        return ;
    }
    #if 0//we don't set it this time.
    ret = neul_bc95_set_cdpserver("0.0.0.0");
	if (ret)
    {
        while(1){} ;
    }
    #endif
	/* Init led matrix hardware driver */
	LOS_TaskLock();
	LOS_MAX7219_Init();
	LOS_TaskUnlock();
	//LOS_MAX7219_show('3');
	//wait socket ok
	osDelay(1000);

	while(1)
	{
		//0x100 x00
		//todo 4
		/*
		leds = led_get_status();
		if (leds)
		{
			data[0] = 0xAA;
			data[1] = 0x72;
			data[2] = 0x00;//00 means device data up
			data[3] = 0x00;
			data[4] = 0x00;//led on
			data[5] = 0x10;//led brightness
		}
		else
		{
			data[0] = 0xAA;
			data[1] = 0x72;
			data[2] = 0x00;//00 means device data up
			data[3] = 0x00;
			data[4] = 0x01;//led off
			data[5] = 0x10;//led brightness
		}
		*/
		data[0] = 0xAA;
		data[1] = 0x72;
		data[2] = 0x00;//00 means device data up
		data[3] = 0x00;
		data[4] = 0x00;//led on
		//data[5] = 0x10;//led brightness
		if (!nb_get_cmd)
		{
			testd = (testd + 1)%36;
			data[5] = testd;
			showd = max7219_nbget_show_data(data[5]);
			LOS_TaskLock();
			//LOS_MAX7219_show(showd);
			LOS_TaskUnlock();
		}
		else
		{
			data[5] = nb_uset_value;
			testd = nb_uset_value;
		}
        ret = neul_bc95_send_coap_paylaod((const char *)data, 6);

		osDelay(5000);
	}

}

void LOS_OceanCon_NB_Upload(void)
{
	osThreadDef_t thread_def;
	
	thread_def.name = "OceanSampleUp";
	thread_def.stacksize = 1900;
	thread_def.tpriority = osPriorityLow;
	thread_def.pthread = (os_pthread)oceancon_nb_task;
	osThreadCreate(&thread_def, NULL);
}

static char coapmsg[536] = {0};
void ocean_recv_dev_cmd(unsigned char *pucVal, int nLen)
{
	char showd;
	//here we should get the cmd and control the led_status

	if (NULL == pucVal || 0 == nLen)
	{
		return ;
	}
	if (3 == nLen && pucVal[0] == 0xaa && pucVal[1] == 0x72)
	{
		if (pucVal[2] > 35)
		{
			nb_get_cmd = 0;
			nb_uset_value = 0;
		}
		else
		{
			nb_get_cmd = 1;
			nb_uset_value = pucVal[2];
			showd = max7219_nbget_show_data(pucVal[2]);
			LOS_TaskLock();
			//LOS_MAX7219_show(showd);
			LOS_TaskUnlock();
		}
	}
	else
	{
		return ;
	}
	
	return ;
}

static UINT32 ocean_nb_indata_process(UINT32 uwParam1,
				UINT32 uwParam2,
				UINT32 uwParam3,
				UINT32 uwParam4)
{
	int n = 0;

	while(1)
	{
        n = neul_bc95_read_coap_msg(coapmsg, 1024);

		if(n <= 0)
		{
			osDelay(100);;
		}
		else
		{
			//do recive cmd
            ocean_recv_dev_cmd((unsigned char *)coapmsg, n);
        }
	}
	return 0;
}

void LOS_OceanCon_NB_data_process(void)
{
	osThreadDef_t thread_def;
	
	thread_def.name = "OceanSampleData";
	thread_def.stacksize = 1024;
	thread_def.tpriority = osPriorityLow;
	thread_def.pthread = (os_pthread)ocean_nb_indata_process;
	osThreadCreate(&thread_def, NULL);
}

void oceancon_nb_sampletask(void * pvParameters)
{
	unsigned char data[8] = {0};
	//int leds;
	int testd = 0;
	char showd;
    int ret = 0;
    int n = 0;
    int rmsgcnt = 0;
    
    /* neul bc95 use uart to trans data , so we need init uart */
    ret = los_dev_uart_init(LOS_STM32L452_UART2, 9600, los_uart_dualbuf, LOS_DEV_UART_DUALBUF_SIZE);
    if (ret != 0)
    {
        while(1);
    }
    
    while(1)
    {    
        while(1)
        {
            //init nb chips
            ret = neul_bc95_hw_init();
            if (!ret)
            {
                //init bc 95 ok
                ret = neul_bc95_get_netstat();
                while(ret != 0)
                {
                    //get network attacted flag
                    ret = neul_bc95_get_netstat();
                    if (0 == ret)
                    {
                        ret = neul_bc95_query_ip();
                    }
                    osDelay(1000);
                }
                break;
            }
            else
            {
                //init failed
                continue ;
            }
        }
        
        /* Init led matrix hardware driver */
        LOS_TaskLock();
        LOS_MAX7219_Init();
        LOS_TaskUnlock();

        osDelay(1000);

        while(1)
        {
            data[0] = 0xAA;
            data[1] = 0x72;
            data[2] = 0x00;//00 means device data up
            data[3] = 0x00;
            data[4] = 0x00;//led on
            //data[5] = 0x10;//led brightness
            if (!nb_get_cmd)
            {
                testd = (testd + 1)%36;
                data[5] = testd;
                showd = max7219_nbget_show_data(data[5]);
                LOS_TaskLock();
                //LOS_MAX7219_show(showd);
                LOS_TaskUnlock();
            }
            else
            {
                data[5] = nb_uset_value;
                testd = nb_uset_value;
            }
            neul_bc95_prepare_transmit();
            ret = neul_bc95_send_coap_paylaod((const char *)data, 6);

            rmsgcnt = neul_bc95_get_unrmsg_count();
            while(rmsgcnt > 0)
            {
                n = neul_bc95_read_coap_msg(coapmsg, 1024);
                if(n > 0)
                {
                    //do recive cmd
                    ocean_recv_dev_cmd((unsigned char *)coapmsg, n);
                }
                rmsgcnt--;
            }
            //send err happened, bc95 can't correct itsself, so redo init
            if (ret < 0)
            {
                break;
            }
            osDelay(5000);
        }
        //some err happened , so redo bc95 init process
    }
}

void LOS_OceanCon_NB_Sample(void)
{
	osThreadDef_t thread_def;
	
	thread_def.name = "OceanSample";
	thread_def.stacksize = 1024;
	thread_def.tpriority = osPriorityLow;
	thread_def.pthread = (os_pthread)oceancon_nb_sampletask;
	osThreadCreate(&thread_def, NULL);
}
