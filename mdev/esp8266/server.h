#ifndef ESP8266_SERVER_H
#define ESP8266_SERVER_H

#include <stdint.h>
#include "mnet/mnet_state.h"

/* Server Socket state */
enum ESP8SSState {
	ESP8SS_IDLE,
	ESP8SS_CIPMUX_MUL,
	ESP8SS_SERVER_UP,
	ESP8SS_SERVER_DOWN,
	ESP8SS_READY,
};

struct SSockStream {
	uint8_t open;
	uint16_t port;
};

MnetState esp8266_server(struct SSockStream *sss);

#endif
