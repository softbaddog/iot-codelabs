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
