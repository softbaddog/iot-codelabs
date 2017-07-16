#ifndef _OCEAN_INTERFACE_H
#define _OCEAN_INTERFACE_H

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

int ocean_init_iot_addr(unsigned char ipaddr[4], unsigned char *port);
// init ocean connect resource, and remote iot server address and port
int ocean_res_init(CON_TYPE_E contype, TRAN_DEV_E trandev, NB_DEV_LIST_E nbdev);
int ocean_get_unread_pkg(void);
// receive data from nb or agenttiny 
int ocean_recv_data(unsigned char *rcvbuf, unsigned short rcvlen);
// send data to IoT platform
int ocean_send_data(unsigned char *sndbuf, unsigned short sndlen);
//deal with the device control command, 
int ocean_dev_control(unsigned char *rcvbuf, unsigned short rcvlen);

#endif
