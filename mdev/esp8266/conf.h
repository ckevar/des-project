#ifndef ESP8266_CONF_H
#define ESP8266_CONF_H

#include "mnet/mnet_state.h"

enum ESP8Confi {
	CHECK_DEV_PRESENCE,
	MUTE_CMD_ECHOING,
	SET_DEV_AS_STATION,
	ALLOW_SINGLE_CONN,
	CHECK_DHCP,
	CONF_DONE,
};

MnetState esp8266_conf(void);

#endif
