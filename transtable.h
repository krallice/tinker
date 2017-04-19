/* 
 * File: transtable.h
 * Author: github/krallice
 * Date: July 2016
 * Description: TODO
 * 
 * See the file "LICENSE" for the full license governing this code.
*/

#ifndef TRANSTABLE_H
#define TRANSTABLE_H

#include "tinker.h"
#include "iptable.h"

// Linked list for our transaction table:
typedef struct trans_tb {
	unsigned char trans[4];
	unsigned char clientmac[6];
	unsigned int count;
	unsigned int state;
	unsigned int timestamp;
	struct ip_tb * ip_offer;
	struct trans_tb * next;
} trans_tb_t;

// Function Prototypes:
void print_transaction_table(trans_tb_t *head);
void purge_transaction_table(trans_tb_t *head);
trans_tb_t *add_transaction_table(trans_tb_t *head, unsigned char* t, unsigned char* m);
void append_transaction_table(trans_tb_t *head, unsigned char* t);

#endif // TRANSTABLE_H
