/* 
 * File: iface.h
 * Author: github/krallice
 * Date: July 2016
 * Description: TODO
 * 
 * See the file "LICENSE" for the full license governing this code.
 * Heavily "borrowed" from http://stackoverflow.com/questions/16710040/arp-request-and-reply-using-c-socket-programming, thanks Mikko Korkalo
*/

#ifndef IFACE_H
#define IFACE_H

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>

typedef struct srv_iface {
	int ifindex;
	const char *ifname;
	unsigned char mac[6]; 
} srv_iface_t;

srv_iface_t * init_srv_iface();

#endif // IFACE_H
