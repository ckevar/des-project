#include <stdio.h>
#include <string.h>

#include "drivers/esp8266.h"
#include "esp8266/server.h"
#include "esp8266/esp8266_cmd.h"
#include "esp8266/buffer.h"

MnetState esp8266_server(struct SSockStream *sss) {
	static enum ESP8SSState promise_state = ESP8SS_IDLE;
	uint16_t cmd_len, arg_len;

	switch(promise_state) {
	case ESP8SS_IDLE:
		promise_state = sss->open == 1 
					 	? ESP8SS_CIPMUX_MUL
						: ESP8SS_SERVER_DOWN;
		return MNET_SERVER;

	/* Enables the SERVER as SOCK_STREAM */
	case ESP8SS_CIPMUX_MUL:
		cmd_len = esp8_cmdset(SOCKET_COUNT);
		esp8_charargset(SOCKET_COUNT_ALLOW, SOCKET_MULTIPLE);
		promise_state = ESP8SS_SERVER_UP;
		break;

	case ESP8SS_SERVER_UP:
		cmd_len = esp8_cmdset(SERVSOCK_STREAM);

		arg_len = esp8_strargset(SERVSOCK_PORT, SERVSOCK_PORT, SERVSOCK_PORT_FMT, sss->port);
		if(arg_len >= (UTX_CMD_BUF_SIZE - SERVSOCK_STREAM_CSIZE - AT_))
			return MNET_ERROR;
		
		cmd_len += arg_len;
		esp8_charargset(SERVSOCK_FD, SERVSOCK_OPEN);

		promise_state = ESP8SS_READY;
		break;
	
	case ESP8SS_READY:
		promise_state = ESP8SS_IDLE;
		return MNET_READY;

	/* Disables the SERVER */
	case ESP8SS_SERVER_DOWN:
		cmd_len = esp8_cmdset(SERVSOCK_STREAM);
		esp8_charargset(SERVSOCK_FD, SERVSOCK_CLOSE);
		promise_state = ESP8SS_READY;
		break;

	default:
		return MNET_ERROR;
	}

	esp8_dma_start(&cmd_len);
	return MNET_BUSY;

}

