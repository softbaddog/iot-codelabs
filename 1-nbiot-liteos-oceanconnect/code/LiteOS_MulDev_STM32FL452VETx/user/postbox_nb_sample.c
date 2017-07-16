#include <string.h>
//#include "ocean_sample.h"
#include "los_bsp_led.h"
#include "los_task.h"
#include "cmsis_os.h"
#include "neul_bc95.h"

#include "stm32f1xx_hal.h"
#include "magent.h"

extern RTC_HandleTypeDef hrtc;
static char postboxmsg[536] = {0};
static char *boxmsg = "{\"identifier\":\"863703030246321\",\"msgType\":\"deviceReq\",\"data\":[{\"serviceId\":\"Mail\",\"serviceData\":{\"currentNumber\":%d,\"isOpen\":%d},\"eventTime\":\"%s\"},{\"serviceId\":\"Device\",\"serviceData\":{\"deviceName\":\"box1\"},\"eventTime\":\"%s\"}]}";
static char msgdata[512] = {0};
static char timestr[64]={0};
void postbox_nb_sampletask(void * pvParameters)
{
	
    
	//int leds;
	int testd = 0;
	char showd;
    int ret = 0;
    int n = 0;
    int lastmail = 0;
    int lastopen = 0;
    int curmail = 0;
    int curopen = 0;
    int rmsgcnt = 0;
    int testtim[6];
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    
    dev_magentsenor_Init();
#if 1    
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
#endif    
	while(1)
	{
        //get if new dat in
        #if 0
        LOS_EvbUartWriteStr2("test ok\r\n", 9);
        LOS_EvbUartReadByte2(timestr, 12, 5000);
        if (strlen(timestr) == 12)
        {
            sscanf(timestr, "%02d%02d%02d%02d%02d%02d", &testtim[0], &testtim[1], &testtim[2],
                            &testtim[3], &testtim[4], &testtim[5]);
            sDate.Year = testtim[0];
            sDate.Month = testtim[1];
            sDate.Date = testtim[2];
            sTime.Hours = testtim[3];
            sTime.Minutes = testtim[4];
            sTime.Seconds = testtim[5];
            //HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
            //HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN); 
            HAL_RTC_SetTimeEx(&hrtc, &sDate, &sTime, RTC_FORMAT_BIN);
            memset(timestr, 0, 12);
        }
        #endif
        //need change here
        curmail = dev_get_mailcount();
        curopen = dev_get_mailopen();
        if (lastmail != curmail || lastopen != curopen)
        {
            HAL_RTC_GetTimeEx(&hrtc, &sDate, &sTime, RTC_FORMAT_BIN);
            //HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
            //HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
            sprintf(timestr,"%04d%02d%02dT%02d%02d%02dZ", 
                    sDate.Year+2000,sDate.Month,sDate.Date, 
                    sTime.Hours,sTime.Minutes,sTime.Seconds);
            //LOS_EvbUartWriteStr2(timestr, strlen(timestr));
            sprintf(msgdata, boxmsg, curmail, curopen, 
                    timestr,
                    timestr);
            lastmail = curmail;
            if (lastopen != curopen)
            {
                dev_clear_mailopen();
                lastopen = 0;
            }
            //LOS_EvbUartWriteStr2(msgdata, strlen(msgdata));
            neul_bc95_prepare_transmit();
            ret = neul_bc95_send_coap_paylaod((const char *)msgdata, strlen(msgdata));
        }

        rmsgcnt = neul_bc95_get_unrmsg_count();
        while(rmsgcnt > 0)
        {
            n = neul_bc95_read_coap_msg(postboxmsg, 1024);
            //something to do 
            rmsgcnt--;
        }
		osDelay(500);
	}

}

void LOS_PostBox_NB_Sample(void)
{
	osThreadDef_t thread_def;
	
	thread_def.name = "PostBoxSample";
	thread_def.stacksize = 1024;
	thread_def.tpriority = osPriorityLow;
	thread_def.pthread = (os_pthread)postbox_nb_sampletask;
	osThreadCreate(&thread_def, NULL);
}
