/* 
 * File: arp.h
 * Author: github/krallice
 * Date: July 2016
 * Description: TODO
 * 
 * See the file "LICENSE" for the full license governing this code.
 * Heavily "borrowed" from http://stackoverflow.com/questions/16710040/arp-request-and-reply-using-c-socket-programming, thanks Mikko Korkalo
*/

#ifndef ARP_H
#define ARP_H

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

#endif // ARP_H
