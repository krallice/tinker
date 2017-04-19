/* 
 * File: iptable.h
 * Author: github/krallice
 * Date: July 2016
 * Description: TODO
 * 
 * See the file "LICENSE" for the full license governing this code.
*/

#ifndef IPTABLE_H
#define IPTABLE_H

#include "tinker.h"
#include "transtable.h"

#define START_IP "192.168.2.150"
#define END_IP "192.168.2.165"

// Linked list for our IP Assignment table:

typedef struct ip_tb {
	struct in_addr host;
	unsigned int used;
	unsigned char mac[6];
	struct ip_tb * next;
} ip_tb_t;

// Prototypes:

int allocate_free_ip(ip_tb_t *head, trans_tb_t *ct);
void init_ip_table(ip_tb_t *head);
void print_ip_table(ip_tb_t *head);

#endif // IPTABLE_H
