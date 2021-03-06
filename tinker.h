/* 
 * File: tinker.h
 * Author: github/krallice
 * Date: July 2016
 * Description: TODO
 * 
 * See the file "LICENSE" for the full license governing this code.
*/

#ifndef TINKER_H
#define TINKER_H

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

// Service Specific Defines:
#define TINKER_DEBUG 1 
#define TINKER_BUFLEN 512
#define TINKER_PORT_SERVER 67
#define TINKER_PORT_CLIENT 68

#define TRANSACTION_LIFETIME 45

// Core DHCP Values:
#define DHCP_MSG_REQUEST 1
#define DHCP_HW_ETH 1

// DHCP Options //
// Amount of bytes into the DHCP request the options begin:
#define DHCP_OPTION_START 240

// Our DHCP States:
#define DHCP_DISCOVER 1
#define DHCP_OFFER 2

// Our DHCP Option Numbers:
#define DHCP_OP_MSG_TYPE 53
#define DHCP_OP_HOSTNAME 12

#endif // TINKER_H
