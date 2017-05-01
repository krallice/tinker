/* 
 * File: iptable.c
 * Author: github/krallice
 * Date: July 2016
 * Description: TODO
 * 
 * See the file "LICENSE" for the full license governing this code.
*/

#include "tinker.h"
#include "iptable.h"


// Dynamically allocate and initialise our IP Table:
ip_tb_t * init_ip_table() {

	ip_tb_t * ip_tb_head;

	// Allocate and wipe:
	ip_tb_head = malloc(sizeof(ip_tb_t));
	memset(ip_tb_head,0,sizeof(ip_tb_head));

	// Init our NULL pointer for Linked List:
	ip_tb_head->next = NULL;

	// Head pointer, for us to iterate through and build:
	ip_tb_t * current = ip_tb_head;

	int ret;

	struct in_addr start_address;
	struct in_addr end_address;
	struct in_addr current_address;

	char first_ip[] = START_IP;
	char end_ip[] = END_IP;

	inet_pton(AF_INET, first_ip, &start_address);
	inet_pton(AF_INET, first_ip, &current_address);
	inet_pton(AF_INET, end_ip, &end_address);

	char current_ip[32] = "";

	int i = 0;
	while ( ntohl(current_address.s_addr) <= ntohl(end_address.s_addr) ) {
	
		//Print:
		inet_ntop(AF_INET, &current_address, current_ip, 32);
		//printf("current_ip as string is %s\n", current_ip);

		// Create our linked list node:
		//printf("current is %p\n", current);
		current->next = malloc(sizeof(ip_tb_t));
		memset(current->next,0,sizeof(ip_tb_t));
		current->next->used = 0;
		current->next->next = NULL;
		current->next->host.s_addr = current_address.s_addr;

		// Increment:
		i++;
		current_address.s_addr = htonl(ntohl(start_address.s_addr) + i);
		current = current->next;
	}

	return ip_tb_head;
}


// Find and then allocate a free IP from our table, and place its refaddress into the transaction table:
int allocate_free_ip(ip_tb_t *head, trans_tb_t *ct) {

	// Init & skip first empty head node:
	ip_tb_t * current = head;
	current = current->next;

	char current_ip[32] = "";

	printf("hit allocate_free_ip\n");

	if ( ct->ip_offer != NULL ) {
		inet_ntop(AF_INET, &(ct->ip_offer->host), current_ip, 32);
		printf("IP Already Allocated for Transaction, IP: %s\n", current_ip);
		return 0;
	}

	while ( current != NULL ) {	
		printf("entering main loop of allocate_free_ip, looking for ip\n");
		if ( current->used == 0 ) {
			inet_ntop(AF_INET, &(current->host), current_ip, 32);
			printf("Found Free IP: %s, allocating\n", current_ip);
			current->used = 1;
			ct->ip_offer = current;
			return 0;
		}
		current = current->next;
	}
	return 1;
}

// Diag function to print our table:
void print_ip_table(ip_tb_t *head) {

	if ( TINKER_DEBUG ) {
		printf("Printing IP Table\n");
	}

	ip_tb_t * current = head;

	char current_ip[32] = "";

	while ( current != NULL ) {
		inet_ntop(AF_INET, &(current->host), current_ip, 32);
		printf("IP Node %p -- Used: %d -- IP: %s\n", current, current->used, current_ip);
		current = current->next;
	}
}

