#include "drivers/esp8266.h"
#include "esp8266/buffer.h"
#include "esp8266/status.h"

MnetState esp8266_status(MnetState *onSuccess) {
	uint16_t j;
	uint16_t *i = &espb.rx.i;

	while(espb.rx.data[*i] != 0) {
		j = (*i + 2) % URX_BUF_SIZE;

		switch(ESP8_RESP_HASH(espb.rx.data[*i], espb.rx.data[j])) {
		
		case ESP8_WRAP:	
		
		case ESP8_OK: 
			esp8_buf_purge(4);	// delete "OK\r\n" 
			return *onSuccess;
		
		case ESP8_IPD: 
			return SSOCKET_POLLIN;
	
		case ESP8_ready:
			esp8_buf_purge(5);	// delete "ready"
			return MNET_CONF;
		
		case ESP8_CIPSTA: 
			return *onSuccess;
		
		case ESP8_STATUS:
			esp8_buf_purge(7);	// delete "STATUS:"
			return *onSuccess;

		case ESP8_FAIL:	
			return MNET_ERROR;
		
		// case ESP8_CLOSED: 
		
		case ESP8_ERROR: 
			return MNET_ERROR;
		// case ESP8_GOT_IP:
		}
		
		esp8_buf_purge_unit();
	}
	return MNET_BUSY;
}
