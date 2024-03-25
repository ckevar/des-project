/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		dma
 * 
 * File generated on 2024-02-16 18:40:34.791409
 * With mdev generator version 0.1.3 from 2024-02-15 14:43:40.628882
 ********************************************************************/

#ifdef _MDEV_USE_DMA_

#include "stm32f4xx.h"

#include "mdev.h"
#include "drivers/dma.h"

/*
struct dma_data_s {
	// dma's data 
};

static struct dma_data_s dma_data[MAX_DMA_MINORS];
struct mdev devs_dma[MAX_DMA_MINORS];
*/


DMA_Stream_TypeDef *DMA_Streamx[] = {
	DMA1_Stream0, DMA1_Stream1, DMA1_Stream2, DMA1_Stream3, 
	DMA1_Stream4, DMA1_Stream5, DMA1_Stream6, DMA1_Stream7,
	DMA2_Stream0, DMA2_Stream1, DMA2_Stream2, DMA2_Stream3, 
	DMA2_Stream4, DMA2_Stream5, DMA2_Stream6, DMA2_Stream7
};

/*
 * ## open_mdma(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t open_mdma(uint8_t min) {
	return 1;
}
*/

/*
 * ## close_mdma(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t close_mdma(uint8_t min) {
	return 1;
}
*/

/*
 * ## read_mdma(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t read_mdma(uint8_t min, void *buf, uint16_t size) {
	return 1;
}
*/

/*
 * ## write_mdma(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t write_mdma(uint8_t min, void *buf, uint16_t size) {
	return 1;
}
*/

/*
 * ## _ioctrl_mdma(uint8_t min, void *arg, uint16_t cmd)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
int32_t _ioctrl_mdma_dma_ctrl(DMA_Stream_TypeDef *DMA_Stream, uint32_t min, uint32_t *ctrl) {
	uint32_t channel = mDMA_CHANNEL(min) << 25;
	*ctrl &= 0xFFFFFFFE;
	DMA_Stream->CR = 0;
	DMA_Stream->CR = channel | *ctrl;
	return DMA_Stream->CR;
}

int32_t _ioctrl_mdma_dma_bind(DMA_Stream_TypeDef *DMA_Stream, struct mDMA_addr *addr) {	
	DMA_Stream->NDTR = addr->size;
	DMA_Stream->PAR = addr->per_src;
	DMA_Stream->M0AR = addr->dest;
	DMA_Stream->M1AR = addr->dest1;
	return DMA_Stream->NDTR;
}

int32_t _ioctrl_mdma(uint8_t min, void *arg, uint16_t cmd) {
	uint8_t stream = mDMA_STREAM(min);
	
	switch (cmd) {

	case IOC_DMA_CTRL:
		return _ioctrl_mdma_dma_ctrl(DMA_Streamx[stream], min, (uint32_t *) arg);

	case IOC_DMA_CTRL_FIFO:
		DMA_Streamx[stream]->CR &= 0xFFFFFFFE; 
		return DMA_Streamx[stream]->FCR = *((uint32_t *) arg);

	case IOC_DMA_BIND: 
		return _ioctrl_mdma_dma_bind(DMA_Streamx[stream], (struct mDMA_addr *) arg);

	case IOC_DMA_START:
		if (arg != NULL) DMA_Streamx[stream]->NDTR = *((uint32_t *) arg);
		while(!(DMA_Streamx[stream]->CR & 0x01)) 
			DMA_Streamx[stream]->CR |= 1;
		
		return DMA_Streamx[stream]->CR & 0x01;
	}

	return 1;
}


/*
 * ## ModuleInit_mdma()
 *

 */
int8_t ModuleInit_mdma(uint32_t rcc_ahbx) {
	// uint8_t i;

	// if(mdev_init(MAX_DMA_MINORS, devs_dma) < 0)
	// 	return -1;
	/*
	for (i = 0; i < MAX_DMA_MINORS; i++) {
		// Initalize mods[i]' fields
	}
	*/
	RCC->AHB1ENR |= rcc_ahbx;
	return 0;
}



#endif /* _MDEV_USE_DMA_*/
