#ifndef _UDP_COAP_INTERFACE_H
#define _UDP_COAP_INTERFACE_H

int udp_send_data(int socket, unsigned char *buf, unsigned short sndlen);
int udp_read_data(int socket, unsigned char *buf, unsigned short maxrlen, unsigned int timeout);
int udp_create_socket(unsigned char *dstip, unsigned short dstport);
int udp_set_local_addr(unsigned char localip[4], unsigned char gateway[4], unsigned char submask[4]);
int udp_stack_init(void);
#endif /* _UDP_COAP_INTERFACE_H */

