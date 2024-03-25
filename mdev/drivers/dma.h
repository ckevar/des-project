/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		dma
 * 
 * File generated on 2024-02-16 18:40:34.757797
 * With mdev generator version 0.1.3 from 2024-02-15 14:43:40.628882
 ********************************************************************/

#ifdef _MDEV_USE_DMA_


#ifndef DMA_H
#define DMA_H

#include <stdint.h>

#define DMA_MINORS 					16 		// 8 streams per DMA
#define DMA2_STREAM_OFFSET 			8

#define IOC_DMA_CTRL 				0
#define IOC_DMA_BIND 				1
#define IOC_DMA_START				2
#define IOC_DMA_CTRL_FIFO 			4
#define IOC_DMA_AUTO_CLEAN_INT 		5

#define ioctrl_mdma2(mi, cmd, arg) 	_ioctrl_mdma(DMA2_STREAM_OFFSET | mi, arg, cmd)
#define ioctrl_mdma1(mi, cmd, arg) 	_ioctrl_mdma(mi, arg, cmd)

#define mDMA_MKDEV(stream, channel) (((channel) << 4) | (stream))
#define mDMA_CHANNEL(dev)  			((dev >> 4) & 7)
#define mDMA_STREAM(dev)  			(dev & 0xF)

#define ModuleInit_mdma1() 			ModuleInit_mdma(RCC_AHB1Periph_DMA1) 
#define ModuleInit_mdma2()			ModuleInit_mdma(RCC_AHB1Periph_DMA2)

struct mDMA_addr {
	uint32_t size;	
	uint32_t per_src; 	// Periph or Memory source
	uint32_t dest;
	uint32_t dest1;
};

struct mDMA_cr {
	uint32_t CR;
	uint32_t FCR;
};

int8_t ModuleInit_mdma(uint32_t rcc_ahbx);
/*
 * List of operators by default, some devices might need them all.
 * */
// int32_t open_mdma(uint8_t min);
// int32_t close_mdma(uint8_t min);
// int32_t read_mdma(uint8_t min, void *buf, uint16_t size);
// int32_t write_mdma(uint8_t min, void *buf, uint16_t size);
int32_t _ioctrl_mdma(uint8_t min, void *arg, uint16_t cmd);

#endif /*DMA_H*/



#endif /* _MDEV_USE_DMA_*/
