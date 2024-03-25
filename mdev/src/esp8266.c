/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		esp8266
 * 
 * File generated on 2024-03-08 11:22:52.329947
 * With mdev generator version 0.1.6 from 2024-03-08 11:21:26.608107
 ********************************************************************/

#ifdef _MDEV_USE_ESP8266_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mdev.h"

#include "drivers/esp8266.h"
#include "esp8266/buffer.h"
#include "esp8266/esp8_state.h"
#include "esp8266/esp8266_cmd.h"

#include "stm32f4xx_dma.h"
#include "drivers/uart.h"

#include "mnet/mnet_state.h"
#include "mnet/mnet_t.h"

/* 
 * it's "3" because most of the commands start at "AT+<command name>" 
 */
/*
#define AT_							3
#define esp8_cmdset(at_cmd)			(AT_ + at_cmd##_CSIZE); \
									memcpy(espb.c_buf + AT_, at_cmd, at_cmd##_CSIZE)

#define esp8_charargset(offset,arg)	espb.c_buf[AT_ + offset] = arg

#define esp8_strargset(offset, cmd_size, fmt, arg) \
		snprintf(espb.c_buf + AT_ + offset, UTX_CMD_BUF_SIZE - cmd_size - AT_, fmt, arg);

#define esp8_dma_start(len) 		DMA2->HIFCR |= DMA_HIFCR_CFEIF6|DMA_HIFCR_CDMEIF6| \
				   								   DMA_HIFCR_CTEIF6|DMA_HIFCR_CHTIF6| \
												   DMA_HIFCR_CTCIF6; \
									ioctrl_mdma2(UTX_DMA, IOC_DMA_START, len)
*/

struct mesp8266 {
	// mesp8266's data 
	struct mDMA_addr tx_addr;
	struct mDMA_addr rx_addr;
	uint32_t 		 dmaTx_CR;
	uint32_t 		 dmaRx_CR;
	uint8_t 		 curr_link;
	uint16_t		 data_size;
};

static struct mesp8266 esp8;
struct mesp8266Buf espb;

/*
 * ## read_mesp8266(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */

enum ESP8ReadState {
	IPD_IN,
	IPD_READ,
	IPD_OUT,
};

void read_mesp8266_get_link_id(void) {
	esp8_buf_purge(5);				// delete "+IPD,"
	esp8.curr_link = espb.rx.data[espb.rx.i];	// cpy <link ID>
	esp8_buf_purge_unit();			// delete "<link ID>
	esp8_buf_purge_unit();			// 		   ,"
};

void read_mesp8266_get_data_size(void) {
	esp8.data_size = atoi(espb.rx.data + espb.rx.i);

	while(espb.rx.data[espb.rx.i] != ':') 
		esp8_buf_purge_unit();

	esp8_buf_purge_unit();			// 	delete ':'
};

void read_mesp8266_read(uint8_t *buf, uint16_t size) {
	while(size) {
		*buf = espb.rx.data[espb.rx.i];
		esp8_buf_purge_unit();
		buf++;
	}
};

uint16_t read_mesp8266(uint8_t li, uint8_t *buf, uint16_t size) {
	uint16_t read_size;
	static enum ESP8ReadState nx_state = IPD_IN;

	read_size = 0;

	switch(nx_state) {
	case IPD_IN:
		if (li != '+') break;
		if (espb.rx.data[espb.rx.i] != '+') break;

		read_mesp8266_get_link_id();
		*buf = esp8.curr_link;

		read_mesp8266_get_data_size();

		nx_state = IPD_READ;
		read_size = 1;
		break;

	case IPD_READ:
		if(esp8.curr_link != li) 
			break;

		if (esp8.data_size <= size) {
			read_size = esp8.data_size;
			esp8.data_size = 0;
			esp8.curr_link = 0;
			nx_state = IPD_IN;
		}
		else {
			read_size = size;
			esp8.data_size -= size;
		}

		read_mesp8266_read(buf, read_size);
		break;
	
	default:
		esp8.curr_link = 0;
		esp8.data_size = 0;
		nx_state = IPD_IN;
	}
	
	return read_size;
}

/*
 * ## seek_mesp8266(uint8_t min, uint32_t offset, uint8_t whence)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
void seek_mesp8266(uint8_t min, uint32_t offset, uint8_t whence) {
	return 1;
}
*/

/*
 * ## write_mesp8266(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t write_mesp8266(uint8_t min, void *buf, uint16_t size) {
	return 1;
}
*/
/*
 * ## close_mesp8266(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */

int32_t close_mesp8266(uint8_t min) {
	if (min != esp8.curr_link)
		return 1;

	esp8_buf_purge(esp8.data_size);

	while(espb.rx.data[espb.rx.i] != 0)
		esp8_buf_purge_unit();

	// TODO:
	// close the link (curr_link);
	// AT+CIPCLOSE=<esp8.curr_link>\r\n
	return 0;
}


/*
 * ## _ioctrl_mesp8266(uint8_t min, void *arg, uint16_t cmd)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */

/******************************************************************************
 * # _ioctrl_esp8_mode
 */

uint16_t _ioctrl_esp8_bind_ap(char mode, char *credentials) {
	uint16_t cmd_len;
	int16_t arg_len;

	switch(mode) {
	
	case ESP8_STAONLY:
		cmd_len = esp8_cmdset(IWCONFIG);
		break;
	
	case ESP8_APONLY:
	case ESP8_STAAP:
		cmd_len = esp8_cmdset(ESP8_SetAP);
		break;
	}

	arg_len = esp8_strargset(ESP8_AP_SSID, ESP8_xAP_CSIZE, ESP8_AP_SSID_FMT, credentials);

	if(arg_len >= (UTX_CMD_BUF_SIZE - ESP8_xAP_CSIZE - AT_))
		return 0;

	return cmd_len + arg_len;
}

MnetState _ioctrl_esp8_mode(char mode, char *credentials) {
	static enum ESP8StaAP_Conf nx_state = STAAP_ENABLE;
	uint16_t cmd_len;

	switch(nx_state) {
	case STAAP_ENABLE:
		cmd_len = esp8_cmdset(ESP8_MODE);
		esp8_charargset(ESP8_MODE_MODE, mode);
		nx_state = STAAP_BIND;
		break;

	case STAAP_BIND:
		cmd_len = _ioctrl_esp8_bind_ap(mode, credentials);
		nx_state = cmd_len == 0 ? STAAP_ENABLE : STAAP_DONE;
		break;

	case STAAP_DONE:
		nx_state = STAAP_BIND;
		return MNET_READY;

	default:
		return MNET_ERROR;
	}

	esp8_dma_start(&cmd_len);
	return MNET_BUSY;
}

/******************************************************************************
 * 
 * # _ioctrl_esp8_server_up
 *
 */

MnetState _ioctrl_mesp8266(void *arg, uint16_t cmd) {
	switch(cmd) {
	default: 
		return MNET_ERROR;
	}
}

/*
 * ## open_mesp8266(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t open_mesp8266(uint8_t min) {
	return 1;
}
*/

/*
 * ## ModuleInit_mesp8266()
 *

 */
void uart_dma_default_config(void) {
	uint32_t conf_fifo;
	
	/* Tx DMA conf */
	esp8.dmaTx_CR = 0;
	esp8.dmaTx_CR = DMA_PeripheralDataSize_Byte | \
			 		DMA_MemoryDataSize_Byte | \
			 		DMA_Priority_High | \
			 		DMA_DIR_MemoryToPeripheral | \
			 		DMA_MemoryInc_Enable;
	esp8.dmaTx_CR = ioctrl_mdma2(UTX_DMA, IOC_DMA_CTRL, &esp8.dmaTx_CR);

	esp8.tx_addr.per_src = (uint32_t) &USART6->DR;
	esp8.tx_addr.dest = 0;
	esp8.tx_addr.size = 0;
	ioctrl_mdma2(UTX_DMA, IOC_DMA_BIND, &esp8.tx_addr);

	conf_fifo = 0;
	ioctrl_mdma2(UTX_DMA, IOC_DMA_CTRL_FIFO, &conf_fifo);
	

	/* Rx DMA conf */
	esp8.dmaRx_CR = 0;
	esp8.dmaRx_CR = DMA_PeripheralDataSize_Byte | \
					DMA_MemoryDataSize_Byte | \
					DMA_Priority_High | \
					DMA_MemoryInc_Enable | \
					DMA_Mode_Circular;
					/* | DMA_DIR_PeripheralToMemory */
	esp8.dmaRx_CR = ioctrl_mdma2(URX_DMA, IOC_DMA_CTRL, &esp8.dmaRx_CR);
	ioctrl_mdma2(URX_DMA, IOC_DMA_CTRL_FIFO, &conf_fifo);
	
	esp8.rx_addr.per_src = (uint32_t) &USART6->DR;
	esp8.rx_addr.dest = (uint32_t) espb.rx.data;
	esp8.rx_addr.size = URX_BUF_SIZE;
	ioctrl_mdma2(URX_DMA, IOC_DMA_BIND, &esp8.rx_addr);
	ioctrl_mdma2(URX_DMA, IOC_DMA_START, NULL);
	
	/* Start ATx command buffer */
	espb.c_buf[0] = 'A';
	espb.c_buf[1] = 'T';
	USART6->CR3 |= USART_CR3_DMAR | USART_CR3_DMAT;

	/* assign the command buffer to the dma memory address */
	esp8.tx_addr.dest = (uint32_t) espb.c_buf;
	ioctrl_mdma2(UTX_DMA, IOC_DMA_BIND, &esp8.tx_addr);
}


void buffer_init(void) {
	// Explicit cast is to avoid warnings, due to begin is a const.
	espb.rx.i = 0;
	espb.rx.len = URX_BUF_SIZE;
}

void ModuleInit_mesp8266(void) {
	
	ModuleInit_muart();
	uart_dma_default_config();
	buffer_init();
}



#endif /* _MDEV_USE_ESP8266_*/
