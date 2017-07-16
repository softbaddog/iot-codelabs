#include "agent_client_interface.h"
#include "los_bsp_led.h"
#include "los_task.h"
#include "coap.h" 
#include "coap_liteos_port.h"
#include "ocean_porting.h"
#include "max7219_driver_func.h"


static board_callback_t agentfunc;
static coap_callback_t coapfunc;
static int socket = -1;

static int test_led_status = 0;//0 led off, 1 led on

extern int get_cmd;
extern int uset_value;

int agenttiny_set_iotaddrinfo(const char *ipaddr, const char *port)
{
	return coap_set_iotaddr_info(ipaddr, port);
}

char max7219_get_show_data(int idx)
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


int agenttiny_res_init(void)
{
	coap_resource_init();
	//int coap_alloc_source(int *socket, int kind)
	if (coap_alloc_source(&socket, 0))
	{
		//failed create socket
		while(1);
	}
	
	agentfunc.pfnGetSN = user_dev_get_sn;
	agentfunc.pfnDataWrite = user_dev_save_data;
	agentfunc.pfnDataRead = user_dev_load_data;
	agentfunc.pfnRecvCmd = user_recv_cmd;
	agentfunc.pfnSendData = coap_lowlevel_send;
	
	coapfunc.pfnCoapBuild = coap_all_build;
	coapfunc.pfnCoapPktfree = coap_free_pkt;
	coapfunc.pfnCoapPktMalloc = coap_malloc_pkt;
	coapfunc.pfnGetRandom = coap_get_random;
	coapfunc.pfnGetToken = coap_get_token;
	coapfunc.pfnSetCoapHeader = coap_set_header;
	coapfunc.pfnSetCoapOption = coap_set_option;
	coapfunc.pfnSetCoapPayload = coap_set_payload;
	coapfunc.pfnSetCoapToken = coap_set_token;
	//todo
	agentfunc.flashAddr = 0x00ff0000;
	agentfunc.uiLength=256;
	agentfunc.iSocket = socket;
	
	UMini_Initial(agentfunc, coapfunc);

	return 0;
}

void oceancon_task(void * pvParameters)
{
	unsigned char data[8] = {0};
	int leds;
	int testd = 0;
	char showd;
	/* Set the IoT GateWay ip and port */
	//agenttiny_set_iotaddrinfo("58.251.166.192", "5683");
	//agenttiny_set_iotaddrinfo("58.251.166.224", "5683");
    agenttiny_set_iotaddrinfo("112.93.129.156", "5683");
	/* Init coap stack and huawei ocean connect agent */
	agenttiny_res_init();
	
	/* Init led matrix hardware driver */
	LOS_TaskLock();
	LOS_MAX7219_Init();
	LOS_TaskUnlock();
	//LOS_MAX7219_show('3');

	
	//wait socket ok
	osDelay(5000);

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
		if (!get_cmd)
		{
			testd = (testd + 1)%36;
			data[5] = testd;
			showd = max7219_get_show_data(data[5]);
			LOS_TaskLock();
			LOS_MAX7219_show(showd);
			LOS_TaskUnlock();
		}
		else
		{
			data[5] = uset_value;
			testd = uset_value;
		}
		UMini_Report(data, 6);

		osDelay(5000);
	}

}

