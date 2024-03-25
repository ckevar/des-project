
#include <string.h>

#include "drivers/esp8266.h"
#include "esp8266/esp8266_cmd.h"
#include "esp8266/buffer.h"

#include "esp8266/conf.h"

MnetState esp8266_conf(void) {
	static enum ESP8Confi nx_state = CHECK_DEV_PRESENCE;
	uint16_t cmd_len;
	
	switch(nx_state) {
	case CHECK_DEV_PRESENCE:
		espb.c_buf[2] = 13;		// "AT\r"
		espb.c_buf[3] = 10; 	// "AT\r\n"
		cmd_len = 4;
		nx_state = MUTE_CMD_ECHOING;
		break;
	
	case MUTE_CMD_ECHOING:
		espb.c_buf[2] = 'E'; 	// "ATE"
		// write_mesp8266(ESP8_NOECHO, ESP8_NOECHO_CSIZE);
		cmd_len = esp8_cmdset(ESP8_NOECHO);
		nx_state = SET_DEV_AS_STATION;
		break;

	case SET_DEV_AS_STATION:
		espb.c_buf[2] = '+'; 	// "AT+"
		cmd_len = esp8_cmdset(ESP8_MODE);
		esp8_charargset(ESP8_MODE_MODE, ESP8_STAONLY);
		nx_state = CONF_DONE;
		break;

/*	case ALLOW_SINGLE_CONN:
		// Default is single connection or single socket
		cmd_len = esp8_cmdset(SOCKET_COUNT);
		esp8_charargset(SOCKET_COUNT_ALLOW, SOCKET_SINGLE);
		//nx_state = CHECK_DHCP;
		nx_state = CONF_DONE;
		break;
	
	case CHECK_DHCP:
		cmd_len = esp8_cmdset(ESP8_RESTORE);
		nx_state = CONF_DONE;
		break;
*/	
	case CONF_DONE:
		/* 
		 * If the routine of CONF is called again, the states gets 
		 * restarted at CHECK_DEV_PRESENCE
		 */
		nx_state = CHECK_DEV_PRESENCE;
		return MNET_CONN;

	default:
		return MNET_ERROR;
	}

	esp8_dma_start(&cmd_len);
	
	return MNET_BUSY;

}
