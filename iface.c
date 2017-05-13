/* 
 * File: iface.c
 * Author: github/krallice
 * Date: July 2016
 * Description: TODO
 * 
 * See the file "LICENSE" for the full license governing this code.
*/

#include "tinker.h"
#include "iface.h"
#include "arp.h"

srv_iface_t * init_srv_iface() {

        // Allocate and wipe:
	srv_iface_t * srv_iface;
	srv_iface = malloc(sizeof(srv_iface_t));
        memset(srv_iface,0,sizeof(srv_iface_t));

	// Assign Interface name ex. eth0:
	srv_iface->ifname = TINKER_IFNAME;

	return srv_iface;
}

void get_if_info(srv_iface_t * srv){

	if ( TINKER_DEBUG ) {
		printf("Getting Interface info for %s\n", srv->ifname);
	}

	struct ifreq ifr;
	int sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	strcpy(ifr.ifr_name, srv->ifname);

	if (ioctl(sd, SIOCGIFINDEX, &ifr) == -1) {
        	die("SIOCGIFINDEX");
    	}

	// Grab index:
	srv->ifindex = ifr.ifr_ifindex;
	if ( TINKER_DEBUG ) {
		printf("Interface Index Num is %d\n", srv->ifindex);
	}
	
	// Grab MAC:
	if (ioctl(sd, SIOCGIFHWADDR, &ifr) == -1) {
		die("SIOCGIFHWADDR");
	}
	memcpy(srv->mac, ifr.ifr_hwaddr.sa_data, 6);

	if ( TINKER_DEBUG ) {
	printf("Client MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", srv->mac[0], srv->mac[1], srv->mac[2], srv->mac[3], srv->mac[4], srv->mac[5]);
	}

}

