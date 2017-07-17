#include "udp_coap_interface.h"
#include "microtcpip.h"
#include <string.h>
#include <stdlib.h>

#include "los_base.ph"
#include "los_hwi.h"
#include "los_sem.h"

#include "spi_enc28j60.h"
#include "net.h"

UINT32 g_udp_stack_task;

#define BUFFER_SIZE 512//400
static unsigned char g_net_out[BUFFER_SIZE + 4];
static unsigned char g_net_in[BUFFER_SIZE + 4];
static unsigned int g_r_len = 0;
static unsigned char *g_r_pos = NULL;

extern unsigned char g_macaddr[6];
static unsigned char myip[4] = {192,168,1,100};
static unsigned char gateway[4] = {192,168,1,1};
static unsigned char mask[4] = {255, 255, 255, 0};

#define UDP_DATA_BUF_CNT 4
#define UDP_DATA_BUF_SIZE 256
static unsigned char g_ring_in[UDP_DATA_BUF_CNT*UDP_DATA_BUF_SIZE];
typedef struct _udp_buf_t
{
    unsigned short datalen;// the number of package that already stored
    unsigned char *buf;
    struct _udp_buf_t *next;
}udp_buf_t;

typedef struct _udp_ring_buf_t
{
    unsigned short unused;// the number of package that already stored
    udp_buf_t rfifo[UDP_DATA_BUF_CNT];
    udp_buf_t *rhead;
    udp_buf_t *whead;
    UINT32 g_udp_wsem;
}udp_ring_buf_t;



UINT32 g_udp_rsem;
static udp_ring_buf_t g_ring_data;
static int udp_init_ring_buf()
{
    int i = 0;
    g_ring_data.unused = UDP_DATA_BUF_CNT;
    g_ring_data.rhead = &(g_ring_data.rfifo[0]);
    g_ring_data.whead = &(g_ring_data.rfifo[0]);
    for (i = 0; i < UDP_DATA_BUF_CNT - 1; i++)
    {
        g_ring_data.rfifo[i].next = &(g_ring_data.rfifo[i+1]);
        g_ring_data.rfifo[i].buf = g_ring_in+i*UDP_DATA_BUF_SIZE;
    }
    g_ring_data.rfifo[i].next = &(g_ring_data.rfifo[0]);
    g_ring_data.rfifo[i].buf = g_ring_in+i*UDP_DATA_BUF_SIZE;
    LOS_SemCreate(1,&g_ring_data.g_udp_wsem);
    return 0;
}

int udp_send_data(int socket, unsigned char *buf, unsigned short sndlen)
{
    (void)socket;
    make_udp_and_send_pkg(g_net_out, (char *)buf, sndlen);
    return sndlen;
}
int udp_read_data(int socket, unsigned char *buf, unsigned short maxrlen, unsigned int timeout)
{
    int rlen = 0;
    UINT32 uwRet;
    //wait semphore , if no timeout then read out the data to coap
    if (g_ring_data.unused == UDP_DATA_BUF_CNT)
    {
        //no data comes
        uwRet = LOS_SemPend(g_udp_rsem, timeout);
        if (uwRet == LOS_OK)
        {
            if (g_ring_data.unused == UDP_DATA_BUF_CNT)
            {
                //no package comes , it's maybe a bug
                rlen = 0;
                return rlen;
            }
            if(maxrlen > g_ring_data.rhead->datalen)
            {
                rlen = g_ring_data.rhead->datalen;
                memcpy(buf, g_ring_data.rhead->buf, rlen);

            }
            else
            {
                rlen = maxrlen;
                //memcpy(buf, g_r_pos, maxrlen);
                memcpy(buf, g_ring_data.rhead->buf, rlen);
                //here we discard the unread data.
            }
            LOS_SemPend(g_ring_data.g_udp_wsem, 65535);
            g_ring_data.unused++;
            g_ring_data.rhead = g_ring_data.rhead->next;
            LOS_SemPost(g_ring_data.g_udp_wsem);
        }
        if (uwRet == LOS_ERRNO_SEM_TIMEOUT)
        {
            //no data
            rlen = 0;
        }
    }
    else
    {
        //there are some data here
        if(maxrlen > g_ring_data.rhead->datalen)
        {
            rlen = g_ring_data.rhead->datalen;
            memcpy(buf, g_ring_data.rhead->buf, rlen);

        }
        else
        {
            rlen = maxrlen;
            //memcpy(buf, g_r_pos, maxrlen);
            memcpy(buf, g_ring_data.rhead->buf, rlen);
            //here we discard the unread data.
        }
        LOS_SemPend(g_ring_data.g_udp_wsem, 65535);
        g_ring_data.unused++;
        g_ring_data.rhead = g_ring_data.rhead->next;
        LOS_SemPost(g_ring_data.g_udp_wsem);
    }
	return rlen;
}

static int is_in_same_network(unsigned char *dstip)
{
    int i = 0;
    int ret = 0;
    for (i = 0; i < 4; i++)
    {
        if ((dstip[i]&mask[i]) != (myip[i]&mask[i]))
        {
            return 0;
        }
    }
    return 1;
}
//dstip is 4 bytes network byte order
int udp_create_socket(unsigned char *dstip, unsigned short dstport)
{
    int socket = 0;

    socket = create_micro_socket(dstip, dstport);
    //request gateway mac addr for udp send myip mask
    if (is_in_same_network(dstip))
    {
        make_arp_request(dstip, g_net_out);
    }
    else
    {
        make_arp_request(gateway, g_net_out);
    }
	return socket;
}

void udp_stack_process(void * pvParameters)
{
    unsigned int plen;
    unsigned char *buf = g_net_in;
    unsigned short payloadlen = 0;
    
    udp_init_ring_buf();
    
    /* Init mac address and local ip address */
	enc28j60Init(g_macaddr);
	init_ip_arp_udp_tcp(g_macaddr,myip);
    //ethernet light:0x476 is PHLCON LEDA(green)=links status, LEDB(red)=receive/transmit
    enc28j60PhyWrite(PHLCON,0x7a4);	
	enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
    
    
    while (1)
    {
        //read data from enc28j60
        plen = enc28j60PacketReceive(BUFFER_SIZE, buf);
        if(plen == 0)
        {
            //no package read out , so delay some time
            LOS_TaskDelay(10);
            continue;
        }
        if(eth_type_is_arp_and_my_ip(buf,plen))
        {
            //get arp request back , we get the gateway mac address
            if (buf[ETH_ARP_OPCODE_H_P] == ETH_ARP_OPCODE_REPLY_H_V
                && buf[ETH_ARP_OPCODE_L_P] == ETH_ARP_OPCODE_REPLY_L_V)
            {      
                get_dest_mac(buf);
            }
            if (buf[ETH_ARP_OPCODE_H_P] == ETH_ARP_OPCODE_REQUEST_H_V
                && buf[ETH_ARP_OPCODE_L_P] == ETH_ARP_OPCODE_REQUEST_L_V)
            {
                make_arp_answer_from_request(buf);
            }
            continue;
        }
        if(eth_type_is_ip_and_my_ip(buf,plen)==0)
        {
            //data is not for us, so discard it
            continue;
        }
        //get udp data , so we should devliver it to coap statck
        //if (buf[IP_PROTO_P]==IP_PROTO_UDP_V&&buf[UDP_DST_PORT_H_P]==4&&buf[UDP_DST_PORT_L_P]==0xb0)
        if (buf[IP_PROTO_P]==IP_PROTO_UDP_V)
        {
            payloadlen = buf[UDP_LEN_H_P];
            payloadlen = (payloadlen << 8)&0xff00;
            payloadlen += buf[UDP_LEN_L_P];
			payloadlen -= UDP_HEADER_LEN;
            g_r_len = payloadlen;
            g_r_pos = buf + UDP_DATA_P;
            
            LOS_SemPend(g_ring_data.g_udp_wsem, 65535);
            if (g_ring_data.unused == 0)
            {
                //discard the package
                LOS_SemPost(g_ring_data.g_udp_wsem);
                continue;
            }
            g_ring_data.unused--;
            if (g_r_len > UDP_DATA_BUF_SIZE)
            {
                //need some optimize
                g_r_len = UDP_DATA_BUF_SIZE;
            }
            memcpy(g_ring_data.whead->buf, g_r_pos, g_r_len);
            g_ring_data.whead->datalen = g_r_len;
            g_ring_data.whead = g_ring_data.whead->next;
            LOS_SemPost(g_ring_data.g_udp_wsem);
      
            LOS_SemPost(g_udp_rsem);
            payloadlen = 0;
		}
    }
}

int udp_set_local_addr(unsigned char localip[4], unsigned char gateway[4], unsigned char submask[4])
{
    memcpy(myip, localip, 4);
    memcpy(gateway, gateway, 4);
    memcpy(mask, submask, 4);
    
    return 0;
}

int udp_stack_init(void)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stInitParam;
    
    stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)udp_stack_process;
    stInitParam.usTaskPrio = 10;
    stInitParam.pcName = "spiethudp";
    stInitParam.uwStackSize = 0x400;
    stInitParam.uwResved   = LOS_TASK_STATUS_DETACHED;
    uwRet = LOS_TaskCreate(&g_udp_stack_task, &stInitParam);
    if (uwRet != LOS_OK)
    {
        return -1;
    }
    
    LOS_SemCreate(0,&g_udp_rsem);
    
    return 0;
}

