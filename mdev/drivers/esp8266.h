/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		esp8266
 * 
 * File generated on 2024-03-08 11:22:52.328890
 * With mdev generator version 0.1.6 from 2024-03-08 11:21:26.608107
 ********************************************************************/

#ifdef _MDEV_USE_ESP8266_


#ifndef ESP8266_H
#define ESP8266_H

#include "mnet/mnet_state.h"
#include "drivers/dma.h"
#include "stm32f4xx_dma.h"

#define ESP8266_MINORS 1	// [Edit] to include more minors

/* IO CTRLS */
#define ioctrl_mesp8266(cmd, arg) 	_ioctrl_mesp8266(arg, cmd)

/* DMA devices */
#define UTX_DMA 					mDMA_MKDEV(6, 5) // Stream 6, channel 5
#define URX_DMA 					mDMA_MKDEV(1, 5) // Stream 1, channel 5

#define esp8_dma_start(len) 		DMA2->HIFCR |= DMA_HIFCR_CFEIF6|DMA_HIFCR_CDMEIF6| \
				   								   DMA_HIFCR_CTEIF6|DMA_HIFCR_CHTIF6| \
												   DMA_HIFCR_CTCIF6; \
									ioctrl_mdma2(UTX_DMA, IOC_DMA_START, len)

void ModuleInit_mesp8266();

/*
 * List of operators by default, some devices might need them all.
 * */

uint16_t read_mesp8266(uint8_t min, uint8_t *buf, uint16_t size);
// void seek_mesp8266(uint8_t min, uint32_t offset, uint8_t whence);
// int32_t write_mesp8266(uint8_t min, void *buf, uint16_t size);
// int32_t close_mesp8266(uint8_t min);
MnetState _ioctrl_mesp8266(void *arg, uint16_t cmd);
// int32_t open_mesp8266(uint8_t min);

#endif /*ESP8266_H*/



#endif /* _MDEV_USE_ESP8266_*/
