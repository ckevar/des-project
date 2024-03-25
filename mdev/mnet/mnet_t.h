#ifndef _MNET_TYPE_H_
#define _MNET_TYPE_H_

#define NET_ADDR_v4_SIZE 	16		/* IPv4: sizeof("255.255.255.255\0") */

// IPv4 Address structure
struct NetAddr4 {
	char ip[NET_ADDR_v4_SIZE];
	char gateway[NET_ADDR_v4_SIZE];
	char netmask[NET_ADDR_v4_SIZE];
};

#endif
