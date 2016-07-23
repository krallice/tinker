/* 
 * File: tinker.h
 * Author: github/krallice
 * Date: July 2016
 * Description: TODO
 * 
 * See the file "LICENSE" for the full license governing this code.
*/

#include <stdio.h> // Standard Printing/IO Functions
#include <string.h> // String Manipulation Functions
#include <stdlib.h> // Standard Functions including exit();

// Network / Socket Header Files:
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#define TINKER_DEBUG 1 
#define TINKER_BUFLEN 512
#define TINKER_PORT_SERVER 67
#define TINKER_PORT_CLIENT 68

#define DHCP_MSG_REQUEST 1
#define DHCP_HW_ETH 1

// Amount of bytes into the DHCP request the options begin:
#define DHCP_OPTION_START 240

#define DHCP_OP_MSG_TYPE 53
#define DHCP_OP_HOSTNAME 12
