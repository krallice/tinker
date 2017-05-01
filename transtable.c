/* 
 * File: transtable.c
 * Author: github/krallice
 * Date: July 2016
 * Description: TODO
 * 
 * See the file "LICENSE" for the full license governing this code.
*/

#include "tinker.h"
#include "iptable.h"
#include "transtable.h"


// Initialise our transaction table:
trans_tb_t *init_transaction_table() {

	trans_tb_t * trans_tb_head;

	// Allocate and wipe:
	trans_tb_head = malloc(sizeof(trans_tb_t));
	memset(trans_tb_head->trans, 0, sizeof(trans_tb_head));

	// Clear values:
	trans_tb_head->state = 0;
	trans_tb_head->ip_offer = NULL;
	trans_tb_head->next = NULL;

	return trans_tb_head;
}

// Print our transaction table:
void print_transaction_table(trans_tb_t *head) { 

	trans_tb_t * current = head;
	
	printf("Printing Transaction Table:\n");

	unsigned int ctime = (unsigned int)time(NULL);

	while ( current != NULL ) {

		char allocated_ip[32] = "";

		printf("Node: ");
		printf("Transaction ID: %02x%02x%02x%02x ", 	current->trans[0], 
								current->trans[1], 
								current->trans[2], 
								current->trans[3]);
		printf("Client MAC Address: %02x:%02x:%02x:%02x:%02x:%02x ", current->clientmac[0],
										current->clientmac[1],
										current->clientmac[2],
										current->clientmac[3],
										current->clientmac[4],
										current->clientmac[5],
										current->clientmac[6]);
		// Print our IP:
		if ( current->ip_offer != NULL ) {
                        inet_ntop(AF_INET, &(current->ip_offer->host), allocated_ip, 32);
			printf("Allocated IP: %s ", allocated_ip);

		}
	
		printf("Current Count: %d ", current->count);
		printf("Current State: %d ", current->state);
		printf("Last Update: %d (%d diff) ", current->timestamp, ctime - current->timestamp);
		printf("Current Address: %p ", current);
		printf("Next Address: %p\n", current->next);
		current = current->next;
	}
}

// For transactions that have being going on past our TRANSTABLE_TRANSACTION_LIFETIME, blow them away:
void purge_transaction_table(trans_tb_t *head) {

	// Assign and skip head node:
	trans_tb_t * current = head;

	// Grab our "previous" node aswell:
	trans_tb_t * previous = current;
	current = current->next;

	printf("Purging Stale Records From Transaction Table:\n");
	// Get current time:
	unsigned int ctime = (unsigned int)time(NULL);

	while ( current != NULL ) {

		// Compare time deltas:
		if ( ctime - current->timestamp >= TRANSTABLE_TRANSACTION_LIFETIME ) {

			// Print our diags:
			printf("\n\n\nTransaction ID: %02x%02x%02x%02x ", 	current->trans[0], 
									current->trans[1], 
									current->trans[2], 
									current->trans[3]);
			printf("candidate for destruction, ");
			printf("Last Update: %d (%d diff)\n", current->timestamp, ctime - current->timestamp);
			
			// Unassign IP linked to this transaction:
			printf("Deleting IP\n");
			trans_tb_t * del = current;
			if ( del->ip_offer != NULL ) {
				del->ip_offer->used = 0;
			}

			// Clean up linkage:
			printf("Cleaning up linkage\n");
			
			// Assign the previous node's "next" linkage to the node AFTER the one we just delete:
			previous->next = current->next;
			
			// Increment the current node
			current = current->next;

			// Blow this transaction away:
			printf("deleting current\n");
			free(del);

		// Else, next:
		} else {
			// Increment over:
			previous = previous->next;
			current = current->next;
		}
	}

}

// Add transactions (new or existing) into table:
trans_tb_t *add_transaction_table(trans_tb_t *head, unsigned char* t, unsigned char* m) {

	trans_tb_t * current = head;
	trans_tb_t * last = head;
	int existing = 0;

	// Traverse our existing linked list:
	while ( current != NULL ) {
		//printf("loop\n");
		// If existing:
		if ( memcmp(current->trans, t, 4) == 0 ) {
			printf("Match for %p, count was %d, is now %d\n", current, current->count, current->count+1);
			current->count = current->count + 1;
			current->timestamp = (unsigned int)time(NULL);
			return current;
			existing = 1;
		}
		last = current;
		current = current->next;
	}

	if ( ! existing ) {
		if ( current  == NULL ) {
			printf("Brand New Transaction\n");
			last->next = malloc(sizeof(trans_tb_t));
			memcpy(last->next->trans,t,4);
			memcpy(last->next->clientmac, m, 6);
			last->next->count = 1;
			last->next->timestamp = (unsigned int)time(NULL);
			last->next->ip_offer = NULL;
			last->next->next = NULL;
		}
	}

	return last->next;
}

// Old proto function for add_transaction_table()
void append_transaction_table(trans_tb_t *head, unsigned char* t) {

	trans_tb_t * current = head;

	// Get to the last element:
	while ( current->next != NULL ) {
		current = current->next;
	}
	
	// Create our new element on the end:
	current->next = malloc(sizeof(trans_tb_t));
	memcpy(current->next->trans,t,4);
	current->next->ip_offer = NULL;
	current->next->next = NULL;
}

