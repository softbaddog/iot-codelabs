//#include "ocean_sample.h"
#include "los_bsp_led.h"
#include "los_task.h"
#include "cmsis_os.h"
#include "max7219_driver_func.h"
#include "neul_bc95.h"
#include "los_dev_st_uart.h"

#include "ocean_interface.h"

//#define USE_NB_BC95 1
#define USE_AGENTTINY 1

int get_cmd = 0;
int uset_value = 0;
int scroll = 0;

unsigned char g_local_ip[4] = {192, 168, 3, 50};
unsigned char g_local_gw[4] = {192, 168, 3, 1};
unsigned char g_local_msk[4] = {255, 255, 255, 0};
unsigned char g_macaddr[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
unsigned char g_devsn[16] = "nbdemo999";

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

void ocean_report_cmd_status(int dataflag, int result)
{
    unsigned char data[8] = {0};
    data[0] = 0x4c;
    data[1] = 0x45;
    data[2] = 0x44;//data[0`2] header
    data[3] = 0x03;// data report 
    if (dataflag == 1)
    {
        data[4] = 0x0d;//set char index cmd 
    }
    else
    {
        data[4] = 0x03;//set char scroll cmd
    }
    if (result == 0)
    {
        data[5] = 0x00;
    }
    else
    {
        data[5] = 0x01;
    }
    ocean_send_data(data, 6);
}
//deal with the device control command, 
//this func will be called in other files while use agenttiny
int ocean_dev_control(unsigned char *rcvbuf, unsigned short rcvlen)
{
    unsigned short nLen = rcvlen;
    unsigned char *pucVal = rcvbuf;
    if (4 == nLen && pucVal[0] == 0xaa && pucVal[1] == 0xaa && pucVal[2] == 0x00 && pucVal[3] == 0x00)
    {
        //iot platform returned data , should discard.
        return 0;
    }
    if (6 == nLen && pucVal[0] == 0x4c && pucVal[1] == 0x45 && pucVal[2] == 0x44)
    {
        if (pucVal[3] == 0x03) 
        {
            //cmd comes
            switch(pucVal[4])
            {
                case 0x0d://set showed char 
                    if (pucVal[5] > 35)
                    {
                        uset_value = 0;
                        ocean_report_cmd_status(1, 1);
                        break;
                    }
                    uset_value = pucVal[5];
                    //showd = max7219_get_show_data(uset_value);
                    LOS_TaskLock();
                    //LOS_MAX7219_show(showd);
                    LOS_TaskUnlock();
                    ocean_report_cmd_status(1, 0);
                    break;
                case 0x03://set char scroll
                    if (pucVal[5] == 0x0)
                    {
                        scroll = 0;
                    }
                    else
                    {
                        scroll = 1;
                    }
                    ocean_report_cmd_status(0, 0);
                    break;
                default:
                    break;
            }
            //need send cmd status
        }
        else
        {
            //not correct cmd
            return 0;
        }
    }
    else
    {
        return 0;
    }
    return 0;
}

static unsigned char test_rbuf[512];
void oceancon_sampletask(void * pvParameters)
{
	unsigned char data[8] = {0};
#ifdef USE_AGENTTINY  
    unsigned char iotaddress[4] = {218,4,33,72};
    char *iotport = "5683";
#endif
	//int leds;
	int testd = 0;
	//char showd;
    int ret = -1;
    int n = 0;
    int rmsgcnt = 0;
#ifdef USE_AGENTTINY    
    ret = ocean_init_iot_addr(iotaddress, (unsigned char *)iotport);
    if (ret != 0)
    {
        while(1);
    }
#endif
    ret = -1;
    while(ret != 0)
    {
#ifdef USE_AGENTTINY
        ret = ocean_res_init(CON_AGENT, TRAN_ETH, NB_MAX);
#endif
#ifdef USE_NB_BC95
        ret = ocean_res_init(CON_NB, TRAN_MAX, NB_NEUL_BC95);
#endif
    }
    while(1)
    {
        /* get sensor data start { */
		data[0] = 0x4c;
		data[1] = 0x45;
		data[2] = 0x44;//data[0`2] header
		data[3] = 0x0d;// data report 
		data[4] = 0x00;//led on
		//data[5] = 0x10;//led brightness
        if (scroll == 0)
        {
            //not scroll
            data[5] = 0x0;
            testd = uset_value;
        }
        else
        {
            uset_value = testd;
            testd = (testd + 1)%36;
            data[5] = 0x1;
        }
        data[4] = testd;
        //showd = max7219_get_show_data(data[4]);
        LOS_TaskLock();
        //LOS_MAX7219_show(showd);
        LOS_TaskUnlock();
        /* get sensor data end } */
        
        //call interface to send data to iot platform
        ocean_send_data(data, 6);
        
        rmsgcnt = ocean_get_unread_pkg();
        while(rmsgcnt > 0)
        {
            n = ocean_recv_data(test_rbuf, 512);
            if (n > 0)
            {
                ocean_dev_control(test_rbuf, n);
            }
            rmsgcnt--;
        }
        osDelay(1000);
    }

}

void los_oceancon_sample(void)
{
	osThreadDef_t thread_def;
	
	thread_def.name = "OceanSample";
	thread_def.stacksize = 2048;
	thread_def.tpriority = osPriorityLow;
	thread_def.pthread = (os_pthread)oceancon_sampletask;
	osThreadCreate(&thread_def, NULL);
}
