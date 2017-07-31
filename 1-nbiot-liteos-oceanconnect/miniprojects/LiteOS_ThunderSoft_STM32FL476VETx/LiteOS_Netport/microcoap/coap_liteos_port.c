#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 0
#include "lwip/tcp.h"
#include "lwip/opt.h"
#include "lwip/tcp.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#endif

#include "los_bsp_led.h"
#include "los_task.h"
#include "cmsis_os.h"
#include "umini.h"
#include "main.h"
#include "coap_liteos_port.h"
#include "los_tick.h"
#include "udp_coap_interface.h"


#define COAP_PKT_MAX 8
#define COAP_RCV_SND_LEN 256
#define COAP_TASK_PRIO 10
#define PORT 5683

#define MEMP_NUM_NETCONN 2

unsigned char g_IotIp[IPLENMAX] = "183.2.20.46";
unsigned char g_IotPort[PORTLENMAX] = "5683";

static coap_res_t coap_resource[MEMP_NUM_NETCONN];
static UINT32 tmrtaskhandle;


static coap_packet_t g_pkt_resource[COAP_PKT_MAX];
static unsigned char pkt_used[COAP_PKT_MAX] = {0};
//static unsigned char g_opt_buf[128] = {0};
static uint8_t scratch_raw[128];
static char g_socket_outbuf[128] = {0};
static UINT32 udp_client(UINT32 uwParam1,
				UINT32 uwParam2,
				UINT32 uwParam3,
				UINT32 uwParam4)
{
	int idx;
	int sock;
	int n = 0;
	//sendto 中使用的对方地址
	//struct sockaddr_in toAddr;
	//recvfrom中使用的对方主机地址
	//struct sockaddr_in fromAddr;
	//socklen_t fromLen;
	
	int rc;
	coap_packet_t pkt;
	coap_packet_t rsppkt;
	size_t rsplen;
	coap_rw_buffer_t scratch_buf;
	UM_RET ret = UM_OK;
	//int test =0;
	//char recv|Buffer[128]
	///coap_packet_t pkt;
	
	idx = (int)uwParam1;
	if(idx < 0 || idx >= MEMP_NUM_NETCONN)
	{
		return (UINT32)-1;
	}
	sock = coap_resource[idx].socket;
	if(sock < 0)
	{
		return (UINT32)-1;
	}
	#if 0// no need this 
	memset(&toAddr, 0, sizeof(toAddr));
	toAddr.sin_family =  AF_INET;
	toAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	toAddr.sin_port = htons(PORT);

	fromLen = sizeof(fromAddr);
    #endif
	while(1)
	{
		if(sock == -1)
		{
			LOS_TaskDelay(200);
			continue;
		}
        #if 0// need rewrite here 
		n = recvfrom(coap_resource[idx].socket, coap_resource[idx].buf, COAP_RCV_SND_LEN, 0, (struct sockaddr *)&fromAddr, &fromLen);
        #else
        n = udp_read_data(coap_resource[idx].socket, coap_resource[idx].buf, COAP_RCV_SND_LEN, 65535);
        #endif
		//coap_resource[idx].remoteAddr = fromAddr;
        if (n == 0)
        {
            //no data recieved
            continue;
        }
		else if(n < 0)
		{
			//err happend
			//close(sock);
			coap_resource[idx].socket = -1;
			sock = -1;
			break;;
		}
		else
		{
			scratch_buf.len = sizeof(scratch_raw);
			scratch_buf.p = scratch_raw;
			rc = coap_parse(&pkt, (const uint8_t*)coap_resource[idx].buf, n);
			if (0 != rc)
			{
				//bad package, do not care
				continue;
			}
			
			//NetRecv(coap_resource[idx].buf, n);
			ret = UMini_NetRecv(pkt.hdr.t, pkt.hdr.code, /*uint8_t delta*/pkt.opts[0].num, (const uint8_t *)pkt.tok.p, pkt.tok.len);
			if (UM_NON == ret)
			{
				//get ack form server, should do nothing
				continue;
			}
			rsplen = COAP_RCV_SND_LEN + 1;//sizeof(buff)+1;
			coap_handle_req(&scratch_buf, &pkt, &rsppkt);
			rc  = coap_build((uint8_t *)g_socket_outbuf, &rsplen, &rsppkt);
			
			
			if(0 != rc)
			{
				//bad data
				continue;
			}
			else
			{
                #if 0// need rewrite here
				n = sendto(coap_resource[idx].socket,
							g_socket_outbuf,
							rsplen,
							0,
							(struct sockaddr *)&coap_resource[idx].remoteAddr,
							sizeof(struct sockaddr_in));
                #else
                n = udp_send_data(coap_resource[idx].socket, (unsigned char *)g_socket_outbuf, rsplen);
                #endif
				if(n != rsplen)
				{
					continue;
				}
			}
		}
	}
	return 0;
}

static UINT32 agenttiny_timer(UINT32 uwParam1,
				UINT32 uwParam2,
				UINT32 uwParam3,
				UINT32 uwParam4)
{
	
	while(1)
	{
		//TimerPro();
		LOS_TaskDelay(9900);
	}
}

/*
	init coap send recv task
	@kind input param for socket kind, 1 means udp server, 0 means udp client
	@socket output parm, if create socket and task success, it save socket handle
	@return valude : 0 means init ok, -1 means failed;
*/
int coap_set_iotaddr_info(const char *ipaddr, const char * port)
{
	int len = 0;
	if (NULL == ipaddr || NULL == port)
	{
		return -1;
	}
	#if 0
	len = (IPLENMAX>strlen(ipaddr)? strlen(ipaddr): IPLENMAX);
	memcpy(g_IotIp, ipaddr, len);
    
	len = (PORTLENMAX>strlen(port)? strlen(port): PORTLENMAX);
	memcpy(g_IotPort, port, len);
    #else
    memcpy(g_IotIp, ipaddr, 4);
	len = (PORTLENMAX>strlen(port)? strlen(port): PORTLENMAX);
	memcpy(g_IotPort, port, len);
    #endif
	return 0;
}

/*
	init coap send recv task
	@kind input param for socket kind, 1 means udp server, 0 means udp client
	@socket output parm, if create socket and task success, it save socket handle
	@return valude : 0 means init ok, -1 means failed;
*/
int coap_alloc_source(int *socket, int kind)
{
	UINT32 uwRet;
	TSK_INIT_PARAM_S stInitParam;
	int sock = -1;
	int port = 0;
	unsigned char *pucIp = g_IotIp;
	unsigned char *usPort = g_IotPort;
    unsigned long tmpip;
	//sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    #if 0// need rewrite here
	sock = socket(AF_INET, SOCK_DGRAM, 0);
    #else
    port = atoi((const char *)usPort);
    sock = udp_create_socket((unsigned char *)pucIp, port);
    #endif
	if(sock < 0)
	{
		return -1;
	}
	coap_resource[sock].socket = sock;
	//coap_resource[sock].kind = kind;
	memcpy(coap_resource[sock].name, "coap", 4);
	coap_resource[sock].name[4] = (char)(sock+0x30);
	coap_resource[sock].name[5] = 0x00;
    
	#if 0//need rewrite here
	if(coap_resource[sock].remoteAddr.sin_port == 0)
	{
		port = atoi((const char *)usPort);
		coap_resource[sock].remoteAddr.sin_addr.s_addr = inet_addr((const char *)pucIp);
		coap_resource[sock].remoteAddr.sin_port = htons(port);
		coap_resource[sock].remoteAddr.sin_family = AF_INET;
	}
	#endif
    
	switch(kind)
	{
		case 0://create client
		default:
			stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)udp_client;
			stInitParam.usTaskPrio = COAP_TASK_PRIO;
			stInitParam.pcName = coap_resource[sock].name;
			stInitParam.uwStackSize = 1200;//0x400;
			stInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
			stInitParam.auwArgs[0] = sock;
			uwRet = LOS_TaskCreate(&coap_resource[sock].taskhandle, &stInitParam);
			break;
	}
	if(LOS_OK != uwRet)
	{
		//close(sock);
		memset(&coap_resource[sock], 0, sizeof(coap_res_t));
		sock = -1;
		return -1;
	}
	if (NULL != socket)
	{
		*socket = sock;
	}
	return 0;
}

/*
	destroy the resource taht coap created
	@socket input parm, thsi function will close the socket and its related task
*/
int coap_free_source(int socket)
{
	UINT32 uwRet;
	if(socket < 0 || socket >= MEMP_NUM_NETCONN)
	{
		return 0;
	}
	if (coap_resource[socket].socket != -1)
	{
		//close(coap_resource[socket].socket);
		coap_resource[socket].socket = -1;
	}
	
	uwRet = LOS_TaskDelete(coap_resource[socket].taskhandle);
	if (LOS_OK != uwRet)
	{
		//this may cause memory leak, because of task delete failed
		memset(&coap_resource[socket], 0, sizeof(coap_res_t));
		return -1;
	}
	coap_resource[socket].taskhandle = 0;
	memset(&coap_resource[socket].buf, 0, COAP_RCV_SND_LEN);
	return 0;
}
int coap_resource_init(void)
{
	int i = 0;
	for(i = 0; i < MEMP_NUM_NETCONN; i++)
	{
		coap_resource[i].socket = -1;
		coap_resource[i].taskhandle = 0;
		coap_resource[i].name[0] = 0x0;
	}
	return 0;
}
int LOS_OceanCon_Login(void)
{
	UINT32 uwRet;
	TSK_INIT_PARAM_S stInitParam;
	
	stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)agenttiny_timer;
	stInitParam.usTaskPrio = COAP_TASK_PRIO;
	stInitParam.pcName = "coaptimer";
	stInitParam.uwStackSize = 800;//1024;//0x400;
	stInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&tmrtaskhandle, &stInitParam);
	return uwRet;
}


static int coap_internal_getsock(UM_SOCK hSock)
{
	int fd;
	fd = (int)hSock;
	return fd;
}
//typedef void (*PFN_SENDDATA)(UM_SOCK hSock, UM_UINT8 *pucVal, UM_SIZE nLen, UM_UINT8 *pucIp, unsigned char * usPort);//Tiny回调设备应用的数据上报数据
void coap_lowlevel_send(UM_SOCK hSock, UM_UINT8 *pucVal, UM_SIZE nLen)
{
	int socket = -1;
	int n = 0;
	int port = 0;
	unsigned char *pucIp=g_IotIp, *usPort=g_IotPort;
	/* may be here we should do alloc source */
	socket = coap_internal_getsock(hSock);
	if (socket < 0 || socket >= MEMP_NUM_NETCONN)
	{
		while(1);
	}
	if (NULL == pucIp || NULL == usPort)
	{
		while(1);
	}
    #if 0// need rewrite here
	if (coap_resource[socket].remoteAddr.sin_port == 0)
	{
		port = atoi((const char *)usPort);
		coap_resource[socket].remoteAddr.sin_addr.s_addr = inet_addr((const char *)pucIp);
		coap_resource[socket].remoteAddr.sin_port = htons(port);
		coap_resource[socket].remoteAddr.sin_family = AF_INET;
	}
	n = sendto(coap_resource[socket].socket,
			pucVal,
			nLen,
			0, 
			(struct sockaddr *)&coap_resource[socket].remoteAddr,
			sizeof(struct sockaddr_in));
    #else
    n = udp_send_data(coap_resource[socket].socket, (unsigned char *)pucVal, nLen);
    #endif
	if(n != nLen)
	{
		//("sendto()  函数使用失败了.\r\n");
		coap_free_source(coap_resource[socket].socket);
	}
	return ;
}


unsigned int coap_get_random(void)
{
	unsigned int ret;
	LOS_TickCountGet();
	srand((unsigned)LOS_TickCountGet());
	ret = rand() % RAND_MAX;
	return ret;
}

void *coap_malloc_pkt(void)
{
	int i = 0;
	unsigned short msgid = 0;
	for(i = 0; i < COAP_PKT_MAX; i++)
	{
		if(0 == pkt_used[i])
		{
			pkt_used[i] = 1;
			msgid = (unsigned short)coap_get_random();
			g_pkt_resource[i].hdr.id[0] = (msgid>>8)&0x00ff;
			g_pkt_resource[i].hdr.id[1] = msgid&0x00ff;
			return (void *)&g_pkt_resource[i];
		}
	}
	return (void *)NULL;
}

void coap_free_pkt(UM_COAPPKT pkt)
{
	int i = 0;
	if (NULL == pkt)
	{
		return ;
	}
	for(i = 0; i < COAP_PKT_MAX; i++)
	{
		if(pkt == &g_pkt_resource[i])
		{
			pkt_used[i] = 0;
			memset(&g_pkt_resource[i], 0, sizeof(coap_packet_t));
		}
	}
	return ;
}

int coap_set_header(UM_COAPPKT pkt, UM_UINT8 ver, UM_UINT8 t, UM_UINT8 tkl, UM_UINT8 code)
{
	coap_packet_t *tmppkt;
	if (NULL == pkt)
	{
		return -1;
	}
	tmppkt = (coap_packet_t *)pkt;
	
	tmppkt->hdr.ver = ver;
	tmppkt->hdr.t = t;
	tmppkt->hdr.tkl = tkl;
	tmppkt->hdr.code = code;
	
	return 0;
}

int coap_set_option(UM_COAPPKT pkt, UM_UINT8 optsIndex, UM_UINT16 optionnum, UM_UINT8 *p, UM_UINT8 plen)
{
	coap_packet_t *tmppkt;
	//int option_len = 0;
	//uint8_t *poptbuf;
	//int i = 0;
	//int offset = 0;
	//unsigned short delta = 0;
	
	if (NULL == pkt)
	{
		return -1;
	}
	if (optsIndex >= MAXOPT)
	{
		return -2;
	}
	tmppkt = (coap_packet_t*)pkt;
	tmppkt->opts[optsIndex].buf.len = plen;
	tmppkt->opts[optsIndex].buf.p = p;
	tmppkt->opts[optsIndex].num = optionnum;
	//tmppkt->numopts = optsIndex;
	tmppkt->numopts++;
	if (tmppkt->numopts != (optsIndex + 1))
	{
		return -3;
	}

	return 0;
}




int coap_set_payload(UM_COAPPKT pkt, UM_UINT8 *p, UM_UINT16 plen)
{
	coap_packet_t *tmppkt;
	if (NULL == pkt)
	{
		return -1;
	}
	tmppkt = (coap_packet_t *)pkt;
	tmppkt->payload.len = plen;//strlen((const char *)data);
	tmppkt->payload.p = p;
	
	return 0;
}

int coap_set_token(UM_COAPPKT pkt, UM_UINT8 *pToken, UM_UINT8 pTokenLen)
{
	coap_packet_t *tmppkt;
	if (NULL == pkt)
	{
		return -1;
	}
	tmppkt = (coap_packet_t *)pkt;
	tmppkt->tok.p = pToken;
	tmppkt->tok.len = pTokenLen;
	
	return 0;
	
}


int coap_get_token(UM_COAPPKT pkt, UM_UINT8 *pToken, UM_UINT8 *pTokenlen)
{
	coap_packet_t *tmppkt;
	if (NULL == pkt)
	{
		return -1;
	}
	if (NULL == pToken || NULL == pTokenlen)
	{
		return -2;
	}
	tmppkt = (coap_packet_t *)pkt;
	memcpy(pToken, tmppkt->tok.p, tmppkt->tok.len);
	*pTokenlen = tmppkt->tok.len;
	
	return 0;
}

int coap_all_build(UM_UINT8 *buf, size_t *buflen, UM_COAPPKT *pkt)
{
	int ret = 0;
	coap_packet_t *tmppkt;
	if (NULL == pkt)
	{
		return -1;
	}
	tmppkt = (coap_packet_t *)pkt;
	ret = coap_build(buf, buflen, tmppkt);
	return ret;
}
