#include <stdio.h>
#include <string.h>

#include "drivers/esp8266.h"
#include "esp8266/esp8266_cmd.h"
#include "esp8266/buffer.h"
#include "esp8266/connection.h"

#include "wpa_conf.h"

#define ATTEMPTS_TO_CHECK_CONNECTION 	50

void buf_strcpy(char *addr) {
	while(espb.rx.data[espb.rx.i] != '"') {                                  
		*addr = espb.rx.data[espb.rx.i];
		addr++;
		esp8_buf_purge_unit();
	}
	*addr = 0;
}

void esp8_cipstate_addrcpy(struct NetAddr4 *addr) {
	
	esp8_buf_purge(16); // purge "+CIPSTA_CUR:ip:\""     
	buf_strcpy(addr->ip);

	esp8_buf_purge(24); // purge "\"\r\n+CIPSTA_CUR<DEF>:gateway:\""
	buf_strcpy(addr->gateway);

	esp8_buf_purge(24); // purge "\"\r\n+CIPSTA_CUR:netmask:\""
	buf_strcpy(addr->netmask);

	esp8_buf_purge(9); 	// purge "\"\r\n\r\nOK\r\n"
}

/* 
 * # esp8_ifconfig_processor
 *
 * If ESP8266 isnt connected to an access point, it retrieves 0.0.0.0 as IP  
 * address. However it doesnt tell anything about whether it has internet access
 * or not.
 *
 */

enum ESP8Conn esp8_ifconfig_processor(struct NetAddr4 addr[static 1]) {
	static uint8_t attempts = ATTEMPTS_TO_CHECK_CONNECTION;

	if(memcmp(addr->ip, "0.0.0.0", 7) == 0) {
		
		attempts--; 	// it shall start automatically
						//
		if (attempts) 
			return EXEC_IFCONFIG;
		
		attempts = ATTEMPTS_TO_CHECK_CONNECTION;
		return EXEC_IWCONFIG;
	
	} else {
		attempts = ATTEMPTS_TO_CHECK_CONNECTION;
		return EXEC_NETSTAT;
	}

}


enum ESP8Conn esp8_netstat_processor(void) {
	switch(espb.rx.data[espb.rx.i]) {
	
	case NETSTAT_AP_CONNECTED:
	case NETSTAT_SOCKET_DISCONN:
		esp8_buf_purge(7);
		return CONN_DONE;

	case NETSTAT_SOCKET_CREATED:
		esp8_buf_purge(7);
		return CLOSE_NET_PORTS;
	
	case NETSTAT_NO_AP:
		esp8_buf_purge(7);
		return EXEC_IFCONFIG;
	}
}

uint16_t esp8_iwconfig(const char *credentials) {
	uint16_t cmd_len, arg_len;

	cmd_len = esp8_cmdset(IWCONFIG);
	arg_len = esp8_strargset(IWCONFIG_CSIZE, IWCONFIG_CSIZE, IWCONFIG_FMT, credentials);

	if (arg_len >= UTX_CMD_BUF_SIZE - IWCONFIG_CSIZE - AT_)
		return 0;

	return cmd_len + arg_len;
}

MnetState esp8266_conn(struct NetAddr4 addr[static 1]) {

	static enum ESP8Conn promise_state = EXEC_IWCONFIG;
	uint16_t cmd_len;

	switch(promise_state) {

	case EXEC_IFCONFIG:
		/* Requests IP on the network, returns +CIP... */
		cmd_len = esp8_cmdset(IFCONFIG_A);
		promise_state = IFCONFIG_PROCESSOR;
		break;

	case IFCONFIG_PROCESSOR:
		/* process the ip get from IFCONFIG */
		esp8_cipstate_addrcpy(addr);
		promise_state = esp8_ifconfig_processor(addr);
		return MNET_CONN;
	
	case EXEC_NETSTAT:
		/* List open ports, it returns "STATUS" */
		cmd_len = esp8_cmdset(NETSTAT);
		promise_state = NETSTAT_PROCESSOR;
		break;

	case NETSTAT_PROCESSOR:
		promise_state = esp8_netstat_processor();
		return MNET_CONN;

	case CLOSE_NET_PORTS:
		cmd_len = esp8_cmdset(CLOSE_SOCK_ALL);
		promise_state = CONN_DONE;
		break;

	case EXEC_IWCONFIG:
		cmd_len = esp8_iwconfig(network);
		promise_state = EXEC_IFCONFIG;
		break;

	case CONN_DONE:
		promise_state = EXEC_IFCONFIG;
		return MNET_READY;

	default: 
		return MNET_ERROR;
	}

	esp8_dma_start(&cmd_len);

	return MNET_BUSY;

}

