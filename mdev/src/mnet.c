/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		net
 * 
 * File generated on 2024-03-05 14:48:11.399758
 * With mdev generator version 0.1.5 from 2024-03-05 13:16:39.358976
 ********************************************************************/

#ifdef _MDEV_USE_NET_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mdev.h"
#include "mnet.h"
#include "mnet/mnet_state.h"
#include "mnet/mnet_t.h"
#include "mnet/wpa.h"

#include "eecfg.h"

#include "drivers/esp8266.h"
#include "esp8266/status.h"
#include "esp8266/conf.h"
#include "esp8266/connection.h"
#include "esp8266/server.h"


struct mnet {
	// mnet's data
	struct NetAddr4 	net_addr;
	volatile MnetState 	state;
	char 				ap_credentials[WPA_AUTH_AP];
	char 				sta_credentials[WPA_AUTH];
	struct SSockStream sss;
};

static struct mnet nets;

TASK(mnet_service) {
	static MnetState prev_state = MNET_CONF;

	switch(nets.state) {
	case MNET_READY:
		prev_state = nets.state;
		break;
	
	case MNET_CONF:
		nets.state = esp8266_conf();
		prev_state = MNET_CONF;
		break;
	
	case MNET_CONN:
		nets.state = esp8266_conn(&nets.net_addr);
		prev_state = MNET_CONN;
		break;

	case MNET_SERVER:
		nets.state = esp8266_server(&nets.sss);
		prev_state = MNET_SERVER;
		break;

	case MNET_ERROR:
		break;

	case MNET_BUSY:
		nets.state = esp8266_status(&prev_state);
		break;
	}
}

/*
 * ## open_mnet(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t open_mnet(uint8_t min) {
	return 1;
}
*/

/*
 * ## close_mnet(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t close_mnet(uint8_t min) {
	return 1;
}
*/

/*
 * ## read_mnet(uint8_t min, void *buf, uint16_t size)
 * 
 * It does not support concurrency. in order to support it, dynamic 
 * allocation might be needed (IDEA NOT TESTED)
 *
 */

uint16_t read_mnet(uint8_t li, uint8_t *buf, uint16_t size) {
	return read_mesp8266(li, buf, size);
}

/*
 * ## write_mnet(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t write_mnet(uint8_t min, void *buf, uint16_t size) {
	return 1;
}
*/

/*
 * ## _ioctrl_mnet(uint8_t min, void *arg, uint16_t cmd)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */

uint8_t _ioctrl_mnet_bind_port(struct SSockStream *sss) {
	memcpy(&nets.sss, sss, sizeof(struct SSockStream));

	while(nets.state != MNET_READY && nets.state != MNET_ERROR);
	
	if (nets.state == MNET_ERROR) 
		return 1;
	
	return 0;
}

/******************************************************************************/
#define IPD_KEY			'+'

uint8_t _ioctrl_mnet_listen(void) {
	uint8_t link; 	
	
	while(nets.state != SSOCKET_POLLIN && nets.state != MNET_ERROR);

	if (nets.state == MNET_ERROR) 
		return 0;

	read_mesp8266(IPD_KEY, &link, 1);
	return link;
}
/*****************************************************************************/

uint8_t _ioctrl_mnet(void *arg, uint16_t cmd) {
	if(nets.state != MNET_READY) {
		return 1;
	}

	switch(cmd) {
	
	case BIND_PORT:
		nets.state = MNET_SERVER;
		return _ioctrl_mnet_bind_port((struct SSockStream *) arg);

	case LISTEN:
		nets.state = MNET_BUSY;
		return _ioctrl_mnet_listen();
	}
	return 0;
}


/*
 * ## seek_mnet(uint8_t min, uint32_t offset, uint8_t whence)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
void seek_mnet(uint8_t min, uint32_t offset, uint8_t whence) {
	return 1;
}
*/

/*
 * ## ModuleInit_mnet()
 *
 */

int8_t ModuleInit_mnet() {
	ModuleInit_mesp8266();
	
	nets.state = MNET_CONF;
	SetRelAlarm(Alarm_mnet_service, 1, 40);
	/* 
	 * Period = 40ms is computed from the following assumptions:
	 *  1. Baudrate = 115200
	 *  2. bits to be transfered = 10 (8 bits of data + 1 start + 1 end);
	 *  3. Buffer size = 512
	 * 
	 * Thus, 
	 * 	total bytes to be transmitted = 512 * 10 = 5120 bits;
	 * 
	 * In the best escenario, there are no delays between data transmission.
	 *
	 * Time to fill the buffer = 5120 / 115200 = 0.044 s ~ 40ms
	 * 
	 * Therefore, there is 4 ms to empty the buffer or at least to make some 
	 * room for new incoming data.
	 */

	while(nets.state != MNET_READY);
	
	return 0;
}

#endif /* _MDEV_USE_NET_*/
