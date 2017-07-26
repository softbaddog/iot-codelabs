#include "ocean_interface.h"
#include "los_dev_st_spi.h"
#include "udp_coap_interface.h"
#include "los_dev_st_uart.h"
#include "neul_bc95.h"
#include "agent_client_interface.h"
#include "umini.h"
#include <string.h>

/*
    we assume that we only use one way to connect iot platform
    so we don't care multi device work at the same time , so
    we give global vars to remember this
*/
static CON_TYPE_E g_con_type = CON_MAX;
static TRAN_DEV_E g_tran_dev = TRAN_MAX;
static NB_DEV_LIST_E g_nbdev_cur = NB_MAX;
static unsigned char iotip[4] = {0};
static unsigned char iotport[8] = {0};

extern unsigned char g_local_ip[4];
extern unsigned char g_local_gw[4];
extern unsigned char g_local_msk[4];


#define LOS_DEV_UART_DUALBUF_SIZE 1024
char los_uart_dualbuf[LOS_DEV_UART_DUALBUF_SIZE] = {0};

#if 0
typedef enum _CON_TYPE_E
{
    CON_AGENT,//    connect huawei IoT platform by ethernet 
    CON_NB,//       connect huawei IoT platform by nb microchip 
    CON_USER,//     connect user platform
    CON_MAX
}CON_TYPE_E;

/* transmit */
typedef enum _TRAN_DEV_E
{
    TRAN_ETH,
    TRAN_GPRS,
    TRAN_NB,
    TRAN_MAX
}TRAN_DEV_E;

typedef enum _NB_DEV_LIST_E
{
    NB_NEUL_BC95,
    NB_LIERDA_NB05,
    NB_MAX
}NB_DEV_LIST_E;
#endif

//need called before iot init
int ocean_init_iot_addr(unsigned char ipaddr[4], unsigned char *port)
{
    if (NULL == ipaddr || NULL == port)
    {
        return -1;
    }
    memcpy(iotip, ipaddr, 4);
    memcpy(iotport, port, strlen((const char *)port));
    return 0;
}

// init ocean connect way, and remote iot server address and port
int ocean_res_init(CON_TYPE_E contype, TRAN_DEV_E trandev, NB_DEV_LIST_E nbdev)
{
    int ret = -1;
    switch(contype)
    {
        case CON_AGENT://agenttiny way 
            g_con_type = CON_AGENT;
            switch(trandev)
            {
                case TRAN_ETH:
                    g_tran_dev = TRAN_ETH;
                    ret = 0;
                    break;
                case TRAN_GPRS:
                    //not implement yet
                    return ret;
                case TRAN_NB:
                    //not implement yet
                    g_tran_dev = TRAN_NB;
                    return ret;
                default:
                    return ret;
            }
            break;
        case CON_NB:
            g_con_type = CON_NB;
            g_tran_dev = TRAN_NB;
            g_nbdev_cur = nbdev;
            ret = 0;
            break;
        default:
            return ret;
    }
    
    if (g_con_type == CON_AGENT && g_tran_dev == TRAN_ETH)
    {
        // agenttiny and spi enc28j60  
        //init udp stack
        ret = los_bus_spi_init(LOS_STM32L476_SPI1);
        ret = udp_set_local_addr(g_local_ip, g_local_gw, g_local_msk);
        ret = udp_stack_init();
        LOS_TaskDelay(300);
        agenttiny_set_iotaddrinfo((char *)iotip, (char *)iotport);
        agenttiny_res_init();
        
    }
    else if (g_con_type == CON_AGENT && g_tran_dev == TRAN_GPRS)
    {
        //not implement yet
        return -1;
    }
    else if (g_con_type == CON_NB && nbdev == NB_NEUL_BC95)
    {
        // use neul bc95 microchip to connect iot platform
        ret = los_dev_uart_init(LOS_STM32L476_UART2, 9600, los_uart_dualbuf, LOS_DEV_UART_DUALBUF_SIZE);
        if (ret != 0)
        {
            while(1);
        }
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
                    LOS_TaskDelay(1000);
                }
                break;
            }
            else
            {
                //init failed
                continue ;
            }
            //
        } // end while(1) 
        // init neul bc 95 ok
    }
    return ret;
}

int ocean_get_unread_pkg(void)
{
    int num = 0;
    if (g_con_type == CON_NB && g_nbdev_cur == NB_NEUL_BC95)
    {
        num = neul_bc95_get_unrmsg_count();
    }
    return num;
}

// receive data from nb or agenttiny 
int ocean_recv_data(unsigned char *rcvbuf, unsigned short rcvlen)
{
    int rlen = 0;
    if (g_con_type == CON_NB && g_nbdev_cur == NB_NEUL_BC95)
    {
        rlen = neul_bc95_read_coap_msg((char *)rcvbuf, rcvlen);
    }
    if (g_con_type == CON_AGENT && g_tran_dev == TRAN_ETH)
    {
        //agent tiny , and ethernet , no data can read here
        return 0;
    }
    return rlen;
}
// send data to IoT platform
int ocean_send_data(unsigned char *sndbuf, unsigned short sndlen)
{
    if (g_con_type == CON_NB && g_nbdev_cur == NB_NEUL_BC95)
    {
        neul_bc95_send_coap_paylaod((const char *)sndbuf, sndlen);
        return sndlen;
    }
    if (g_con_type == CON_AGENT)
    {
        UMini_Report(sndbuf, sndlen);
        return sndlen;
    }
    return 0;
}

