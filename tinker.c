/* 
 * File: tinker.c
 * Author: github/krallice
 * Date: July 2016
 * Description: TODO
 * 
 * See the file "LICENSE" for the full license governing this code.
*/

#include "tinker.h"

void die(char *s) {
    perror(s);
    exit(1);
}

void udpListen() {
}

int main (int argc, char **argv[]) {

	if ( TINKER_DEBUG ) {
		printf("Starting tinker dhcp ip server now...\n");
	}

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
		if ( buf[0] == DHCP_MSG_REQUEST ) {
			printf("Recieved DHCP Message type of REQUEST\n");
		}
		if ( buf[1] == DHCP_HW_ETH ) {
			printf("Recieved DHCP Hardware Type of ETHERNET\n");
		}
		printf("Transaction ID: %02x, %02x, %02x, %02x\n", (unsigned)(unsigned char)buf[4], 
								   (unsigned)(unsigned char)buf[5], 
								   (unsigned)(unsigned char)buf[6], 
								   (unsigned)(unsigned char)buf[7]);
		printf("Data:\n");
		int i;
		for ( i = 0; i <= TINKER_BUFLEN; i++ ) {
			printf("%04x " , buf[i]);
		}
		printf("End Data:\n");
		
		// Reset:
		memset(buf, '\0', TINKER_BUFLEN);
	}
	close(s);
	return 0;
}
