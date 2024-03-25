#include <stddef.h>

#include "mnet.h"
#include "mnet/mnet_state.h"
#include "mnet/ssocket.h"

#include "esp8266/server.h"

uint8_t bind_socks(uint16_t port) {
	struct SSockStream sss = {
		.open = 1,
		.port = port,
	};
	return ioctrl_mnet(BIND_PORT, &sss);
}

uint8_t close_socks(uint8_t link) {
	struct SSockStream sss = {
		.open = 0,
	};
	
	return ioctrl_mnet(BIND_PORT, &sss);
}
