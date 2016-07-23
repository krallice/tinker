/* 
 * File: tinker.c
 * Author: github/krallice
 * Date: July 2016
 * Description: TODO
 * 
 * See the file "LICENSE" for the full license governing this code.
*/

#include "tinker.h"

// Linked list for our transaction table:
typedef struct trans_tb {
	unsigned char trans[4];
	unsigned int count;
	struct trans_tb * next;
} trans_tb_t;

void die(char *s) {
    perror(s);
    exit(1);
}

void udpListen() {
}

void print_transaction_table(trans_tb_t *head) { 

	trans_tb_t * current = head;
	
	printf("Printing Transaction Table:\n");

	while ( current != NULL ) {
		printf("Node: ");
		printf("Transaction ID: %02x%02x%02x%02x ", 	current->trans[0], 
								current->trans[1], 
								current->trans[2], 
								current->trans[3]);
		
		printf("Current Count: %d ", current->count);
		printf("Current Address: %p ", current);
		printf("Next Address: %p\n", current->next);
		current = current->next;
	}
}

void add_transaction_table(trans_tb_t *head, unsigned char* t) {

	trans_tb_t * current = head;
	trans_tb_t * last = head;
	int existing = 0;

	// Traverse our existing linked list:
	while ( current != NULL ) {
		printf("loop\n");
		// If existing:
		if ( memcmp(current->trans, t, 4) == 0 ) {
			printf("Match for %p, count was %d, is now %d\n", current, current->count, current->count+1);
			current->count = current->count + 1;
			existing = 1;
		}
		last = current;
		current = current->next;
	}

	if ( ! existing ) {
		if ( current  == NULL ) {
			printf("Brand New Transaction");
			last->next = malloc(sizeof(trans_tb_t));
			memcpy(last->next->trans,t,4);
			last->next->count = 1;
			last->next->next = NULL;
		}
	}
}

void append_transaction_table(trans_tb_t *head, unsigned char* t) {

	trans_tb_t * current = head;

	// Get to the last element:
	while ( current->next != NULL ) {
		current = current->next;
	}
	
	// Create our new element on the end:
	current->next = malloc(sizeof(trans_tb_t));
	memcpy(current->next->trans,t,4);
	current->next->next = NULL;

}

int main (int argc, char **argv[]) {

	if ( TINKER_DEBUG ) {
		printf("Starting tinker dhcp ip server now...\n");
	}
	
	// Init the first transaction table entry for our linked list:
	trans_tb_t * trans_tb_head;
	trans_tb_head = malloc(sizeof(trans_tb_t));
	memset(trans_tb_head->trans, 0, sizeof(trans_tb_head));
	trans_tb_head->next = NULL;

	// Define Sockaddr structures for me (server) and other (clients):
	// sockaddr_in prototype:
	// struct sockaddr_in {
	// short            sin_family;   // e.g. AF_INET
	// unsigned short   sin_port;     // e.g. htons(3490)
	// struct in_addr   sin_addr;     // see struct in_addr, below
	// char             sin_zero[8];  // zero this if you want to
	// };
	// From http://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html: 
	// With IPv4 (what basically everyone in 2005 still uses), the struct s_addr is a 4-byte number that represents one digit in an IP address per byte. 
	// (You won't ever see an IP address with a number in it greater than 255.)
	struct sockaddr_in si_me, si_other;

	int s, i, slen = sizeof(si_other) , recv_len;
	int broadcast = 1;
	char buf[TINKER_BUFLEN];

	// Create a UDP/Datagram Socket: (s)
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		die("socket");
	}

	// Allow Broadcast on Socket:
	if ((setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast)) == -1) {
		die("setsockopt - SO_SOCKET ");
	}

	// Zero out our si_me structure (for safety), and then populate:
	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(TINKER_PORT_SERVER);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	// Bind our UDP Datagram Socket to our sockaddr_in presenting the server:
	if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1) {
		die("bind");
	}

	// Listen for Packets:
	while(1) {

		printf("Waiting for data...\n");
		fflush(stdout);
		 
		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, TINKER_BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1) {
		    die("recvfrom()");
		}
		 
		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

		// Only support ETHERNET Hardware for v1:
		if ( buf[1] == DHCP_HW_ETH ) {
			printf("Recieved DHCP Hardware Type of ETHERNET\n");

			// If DHCP Request:
			if ( buf[0] == DHCP_MSG_REQUEST ) {
				printf("Recieved DHCP Message type of REQUEST\n");

				// Client's MAC Address:
				unsigned char clientmac[6];
				memcpy(clientmac,&buf[28],6);
				printf("Client MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", clientmac[0],
												clientmac[1],
												clientmac[2],
												clientmac[3],
												clientmac[4],
												clientmac[5],
												clientmac[6]);
				// Our transaction ID:
				unsigned char trans[4];
				memcpy(trans,&buf[4],4);
				printf("Transaction ID: %02x%02x%02x%02x\n", trans[0], 
										   trans[1], 
										   trans[2], 
										   trans[3]);

				// Our elapsed seconds for transaction:
				unsigned char sec[2];
				memcpy(sec,&buf[8],2);
				printf("Seconds Elapsed: %02d%02d\n", sec[0], sec[1]);

				// Start playing with our transaction table:
				add_transaction_table(trans_tb_head, trans);
				//append_transaction_table(trans_tb_head, trans);
				print_transaction_table(trans_tb_head);

				// Entire DGRAM dump:
				printf("Data:\n");
				int i;
				for ( i = 0; i <= TINKER_BUFLEN; i++ ) {
					printf("%02x " , (unsigned)(unsigned char)buf[i]);
				}
				printf("End Data:\n");
			
			}
		}
	
	// Reset:
	memset(buf, '\0', TINKER_BUFLEN);
	}

	close(s);
	return 0;
}
