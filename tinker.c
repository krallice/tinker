/* 
 * File: tinker.c
 * Author: github/krallice
 * Date: July 2016
 * Description: TODO
 * 
 * See the file "LICENSE" for the full license governing this code.
*/

// DHCP State Machine:
//# dhclient -v eth2

//DHCPDISCOVER on eth2 to 255.255.255.255 port 67 interval 3 (xid=0x10c1ff2a)
//DHCPOFFER from 192.168.1.1
//DHCPREQUEST on eth2 to 255.255.255.255 port 67 (xid=0x10c1ff2a)
//DHCPACK from 192.168.1.1 (xid=0x10c1ff2a)
//bound to 192.168.1.105 -- renewal in 37569 seconds.

#include "tinker.h"
#include "iptable.h"

// Linked list for our transaction table:
typedef struct trans_tb {
	unsigned char trans[4];
	unsigned int count;
	unsigned int state;
	struct trans_tb * next;
} trans_tb_t;

// Linked list for our IP Assignment table:
typedef struct ip_tb {
	struct in_addr host;
	unsigned int used;
	unsigned char mac[6];
	struct ip_tb * next;
} ip_tb_t;

void die(char *s) {
    perror(s);
    exit(1);
}

void send_dhcp_offer(trans_tb_t *ct) {

	printf("Getting Ready to Send DHCP OFFER\n");
	char a[] = "Under Construction!\n";



}

// Parse our DHCP Options:
int parse_options(unsigned char b[], trans_tb_t *ct) {

	printf("Starting Parser\n");
	printf("First byte is %02x\n", b[0]);

	printf("First option is is %02x\n", b[DHCP_OPTION_START]);

	int i = DHCP_OPTION_START;
	int l;

	while ( b[i] != 0xFF ) {
		printf("i is %d\n", i);
		// DHCP MESSAGETYPE
		if ( b[i] == DHCP_OP_MSG_TYPE ) {
			printf("DHCP OPTION: DHCP MESSAGE TYPE\n");
			// Get our length:
			l = (int)b[i+1];
			printf("DHCP OPTION LENGTH: %d\n", l);
			if ( b[i+2] == DHCP_DISCOVER ) {
				printf("dhcp_discover mode\n");
				ct->state = DHCP_DISCOVER;
			}
		} else if ( b[i] == DHCP_OP_HOSTNAME ) {
			printf("DHCP OPTION: HOSTNAME\n");
			// Get our length:
			l = (int)b[i+1];
			printf("DHCP OPTION LENGTH: %d\n", l);
			char h[l];
			memcpy(&h,&b[i+2],l);
			printf("ADVERTISED HOSTNAME: %s\n", h);
		// Else Unknown Option Type:	
		} else {
			printf("DHCP OPTION: UNKNOWN, NUMBER: %02x\n", b[i]);
			// Get our length:
			l = (int)b[i+1];
			printf("DHCP OPTION LENGTH: %d\n", l);
		}
		i = i + l + 2;
	}
	
	// Based on our parsed options, perform an action:
	if ( ct->state == DHCP_DISCOVER ) {
		return DHCP_OFFER;
	} else {
		return -1;
	}
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
		printf("Current State: %d ", current->state);
		printf("Current Address: %p ", current);
		printf("Next Address: %p\n", current->next);
		current = current->next;
	}
}

trans_tb_t *add_transaction_table(trans_tb_t *head, unsigned char* t) {

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
			last->next->count = 1;
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
	current->next->next = NULL;


}

// Dynamically allocate and initialise our IP Table:
void init_ip_table(ip_tb_t *head) {

	ip_tb_t * current = head;

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

}

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

int main (int argc, char **argv[]) {

	if ( TINKER_DEBUG ) {
		printf("Starting tinker dhcp ip server now...\n");
	}
	
	// Init the first transaction table entry for our linked list:
	trans_tb_t * trans_tb_head;
	trans_tb_head = malloc(sizeof(trans_tb_t));
	memset(trans_tb_head->trans, 0, sizeof(trans_tb_head));
	trans_tb_head->state = 0;
	trans_tb_head->next = NULL;

	// Let's build our IP Table:
	ip_tb_t * ip_tb_head;
	ip_tb_head = malloc(sizeof(ip_tb_t));
	memset(ip_tb_head,0,sizeof(ip_tb_head));
	ip_tb_head->next = NULL;
	init_ip_table(ip_tb_head);
	print_ip_table(ip_tb_head);

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
	unsigned char buf[TINKER_BUFLEN];

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
				
				int ret;
				trans_tb_t * ct;
				ct = add_transaction_table(trans_tb_head, trans);
				if ((ret = parse_options(buf, ct)) == DHCP_OFFER) {
					send_dhcp_offer(ct);
					char a[] = "DHCP_OFFER_PLACEHOLDER_UNDER_CONSTRUCTION";
				}
				//append_transaction_table(trans_tb_head, trans);
				print_transaction_table(trans_tb_head);


				/*
				// Entire DGRAM dump:
				printf("Data:\n");
				int i;
				for ( i = 0; i <= TINKER_BUFLEN; i++ ) {
					printf("%02x " , (unsigned)(unsigned char)buf[i]);
				}
				printf("End Data:\n");
				*/
			
			}
		}
	
	// Reset:
	memset(buf, '\0', TINKER_BUFLEN);
	}

	close(s);
	return 0;
}
