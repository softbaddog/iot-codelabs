#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if 0//no need here
#include "lwip/tcp.h"
#include "lwip/opt.h"
#include "lwip/tcp.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#endif
#include "los_task.h"
#include "cmsis_os.h"
#include "umini.h"
#include "main.h"

#ifndef _COAP_LITEOS_PORT_H
#define _COAP_LITEOS_PORT_H

#define COAP_RCV_SND_LEN 256
#define COAP_TASK_PRIO 10
#define PORT 5683

typedef struct _coap_res_t
{
	int socket;
	//int kind;//socket kind , 0 udp client, 1 udp server
	UINT32 taskhandle;// task handle
	char name[8]; // task name
	//struct sockaddr_in remoteAddr;
	unsigned char buf[COAP_RCV_SND_LEN];//
}coap_res_t;

typedef enum
{
	C_LED_ON,
	C_LED_OFF,
	C_LED_MAX
}COAP_LED_STAT;



static UINT32 udp_client(UINT32 uwParam1,
				UINT32 uwParam2,
				UINT32 uwParam3,
				UINT32 uwParam4);
				
static UINT32 udp_server(UINT32 uwParam1,
				UINT32 uwParam2,
				UINT32 uwParam3,
				UINT32 uwParam4);				

static UINT32 agenttiny_timer(UINT32 uwParam1,
				UINT32 uwParam2,
				UINT32 uwParam3,
				UINT32 uwParam4);
/*
	init coap send recv task
	@kind input param for socket kind, 1 means udp server, 0 means udp client
	@socket output parm , if create socket and task success, it save socket handle 
	@return value : 0 means init ok, -1 means failed;
*/
int coap_alloc_source(int *socket, int kind);

/*
	destroy the resource that coap created
	@socket input parm , this function will close the socket and its related task
*/
int coap_free_source(int socket);


int LOS_OceanCon_Login(void);

int coap_set_iotaddr_info(const char *ipaddr, const char * port);
int coap_resource_init(void);

static int coap_internal_getsock(UM_SOCK hSock);
//void coap_lowlevel_send(UM_SOCK hSock, UM_UINT8 *pucVal, UM_SIZE nLen, UM_UINT8 *pucIp, UM_UINT16 usPort);
void coap_lowlevel_send(UM_SOCK hSock, UM_UINT8 *pucVal, UM_SIZE nLen);


unsigned int coap_get_random(void);
void *coap_malloc_pkt(void);
void coap_free_pkt(UM_COAPPKT pkt);
int coap_set_header(UM_COAPPKT pkt, UM_UINT8 ver, UM_UINT8 t, UM_UINT8 tkl, UM_UINT8 code);
int coap_set_option(UM_COAPPKT pkt, UM_UINT8 optsIndex, UM_UINT16 optionnum, UM_UINT8 *p, UM_UINT8 plen);
int coap_set_payload(UM_COAPPKT pkt, UM_UINT8 *p, UM_UINT16 plen);
int coap_set_token(UM_COAPPKT pkt, UM_UINT8 *pToken, UM_UINT8 pTokenLen);
int coap_get_token(UM_COAPPKT pkt, UM_UINT8 *pToken, UM_UINT8 *pTokenlen);
int coap_all_build(UM_UINT8 *buf, size_t *buflen, UM_COAPPKT *pkt);
#endif
