#ifndef ESP8266_CONNECTION_H
#define ESP8266_CONNECTION_H

#include "mnet/mnet_state.h"
#include "mnet/mnet_t.h"

enum ESP8Conn {
	EXEC_IFCONFIG,
	IFCONFIG_PROCESSOR,
	EXEC_NETSTAT,
	NETSTAT_PROCESSOR,
	EXEC_IWCONFIG,
	IWCONFIG_PROCESSOR,
	CLOSE_NET_PORTS,
	CONN_DONE,
};

MnetState esp8266_conn(struct NetAddr4 addr[static 1]);

#endif
