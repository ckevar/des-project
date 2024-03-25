/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		lcd35rt
 * 
 * File generated on 2024-02-15 14:43:44.983427
 * With mdev generator version 0.1.3 from 2024-02-15 14:43:40.628882
 ********************************************************************/

#ifdef _MDEV_USE_LCD35RT_


#include "mdev.h"
#include "drivers/lcd35rt.h"
#include "drivers/dma.h"

#include "lcd_log.h"

#define LCD35RT_OnDMA 		mDMA_MKDEV(0, 0) 	// LCD is using DMA2, stream 0, channel 0
#define VLCD_HORIZONTAL_MAX (LCD_HORIZONTAL_MAX / 2)
/* Flags to restore the LCD conf */
#define RES_DMA_CR 			0b00000001 			// Default DMA Control Register
#define RES_DMA_ADDR		0b00000010			// Address for DMA
#define RES_LCDD_WIN		0b00000100 			// LCD Driver, Size of Window
#define RES_LCDD_ADDRC		0b00001000 			// LCD Driver, Address counter


struct lcd35rt {
	// lcd35rt's data 
	struct mDMA_addr 		addr;
	uint32_t 				dma_CR;
	uint32_t 				color; 	
	struct ssd2119_window 	win;
	volatile uint8_t 		dma_busy; 		// this might be a semaphore
	volatile uint8_t 		restore;
};
struct lcd35rt lcd_conf;
/*
static struct lcd35rt lcd35rts[MAX_LCD35RT_MINORS];
struct mdev devs_lcd35rt[MAX_LCD35RT_MINORS];
*/

/*
 * ## open_mlcd35rt(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t open_mlcd35rt(uint8_t min) {
	return 1;
}
*/

/*
 * ## close_mlcd35rt(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t close_mlcd35rt(uint8_t min) {
	return 1;
}
*/

/*
 * ## read_mlcd35rt(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t read_mlcd35rt(uint8_t min, void *buf, uint16_t size) {
	return 1;
}
*/

/*
 * ## write_mlcd35rt(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t write_mlcd35rt(uint8_t min, void *buf, uint16_t size) {
	return 1;
}
*/

/*
 * ## _ioctrl_mlcd35rt(uint8_t min, void *arg, uint16_t cmd)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */

void _ioctrl_mlcd35rt_clear(void) {
	/* DMA can transfer max 65535 units of any size (8, 16 o 32). Since LCD is 320*240 = 76800 
	 * pixels of 16 bits depth. a single DMA transfer cannot cover it. But instead, if the transfer
	 * size is set as 32 bits (two consecutive pixels), the number of (virtual) pixels is 38400.
	 * This approach can also make the virtual width of the LCD turn into 160 pixels. However, the
	 * SSD2119 has a capacity for 18 bits per pixels. 
	 * 		words@16bpp = 320*240*16 / 8 = 153600
	 * 		used_capacity%  = total_capacity / 153600 * 100 = 88.88% 
	 * This will be the area covered as well. So, to fully clean the screen, we will use the full
	 * SRAM capacity.
	 * 		size = 172800/4 = 43200
	 * As far as I understand, everytime a line is completed, SSD re-aligns its memory to cast from
	 * RGB565 to RGB888, so the following entry isnt in the same address where the last line ended,
	 * but a bit ahead. 
	 */
	uint32_t DMA_CR;

	seek_mssd2119(0, 0, SEEK_SET);

	lcd_conf.addr.size = 43200;
	lcd_conf.color = 0x00000000;
	DMA_CR = (lcd_conf.dma_CR & 0xFFFF9FFF) | \
			 DMA_PeripheralDataSize_Word | \
			 DMA_MemoryDataSize_Word;
	ioctrl_mdma2(LCD35RT_OnDMA, IOC_DMA_CTRL, &DMA_CR);
	ioctrl_mdma2(LCD35RT_OnDMA, IOC_DMA_START, &(lcd_conf.addr.size));
	
	/* Enable restoration of windows size */
	lcd_conf.restore = RES_DMA_CR;
}

void _ioctrl_mlcd35rt_mhvline(struct lcd35rt_line *line) {
	seek_mssd2119(0, line->origin.pos, SEEK_SET);

	lcd_conf.color = (uint32_t) line->origin.color;
	ioctrl_mdma2(LCD35RT_OnDMA, IOC_DMA_START, &line->length);
}

void _ioctrl_mlcd35rt_mboxfull(struct lcd35rt_boxf *box) {
	uint16_t size;
	struct ssd2119_window win;

	/* set the ssd2119 in window mode */
	win.pos = box->origin.pos;
	win.height = box->height;
	win.width = box->width;
	ioctrl_mssd2119(0, IOC_SSD2119_HWin, &win);

	/* configure DMA */
	lcd_conf.color = (uint32_t) box->origin.color;
	size = win.height * win.width ;
	ioctrl_mdma2(LCD35RT_OnDMA, IOC_DMA_START, &size);
	
	/* Enable restoration of windows size */
	lcd_conf.restore = RES_LCDD_WIN;
}

void _ioctrl_mlcd35rt_565box(lcd35rt_565shape_t *box) {
	/* 
	 * Keep in mind this is a dangerous format, since in the header is written
	 * the image dimensions, so we can extract the image size out of it, it can
	 * be tricked into prentending to be a larger image, thus sending corrupting
	 * files, inroder to avoid this, we think it's important to set in the file
	 * format an standard ending, so before passing the memory to the controller 
	 * it can be verifies that at the end of the "file" there's indeed the
	 * sequence of ending
	 */
	uint16_t size;
	uint32_t DMA_CR;

	/* set the ssd2119 in window mode */
	ioctrl_mssd2119(0, IOC_SSD2119_HWin, &box->win);
	
	/* DMA CONF { */
	DMA_CR = lcd_conf.dma_CR | DMA_PeripheralInc_Enable; 
	ioctrl_mdma2(LCD35RT_OnDMA, IOC_DMA_CTRL, &DMA_CR);
	
	lcd_conf.addr.size = box->win.height * box->win.width;
	lcd_conf.addr.per_src = box->data;
	ioctrl_mdma2(LCD35RT_OnDMA, IOC_DMA_BIND, &lcd_conf.addr);
	ioctrl_mdma2(LCD35RT_OnDMA, IOC_DMA_START, NULL);
	/* } */
	/* */
	lcd_conf.restore = RES_DMA_CR | RES_DMA_ADDR | RES_LCDD_WIN;
}

int32_t _ioctrl_mlcd35rt(uint8_t min, void *arg, uint16_t cmd) {

	/* Busy waiting, dangerous */
	while(lcd_conf.dma_busy);
	lcd_conf.dma_busy = 1;
	/* END danger */

	switch(cmd) {
	case IOC_LCD35RT_CLEAR: 
		_ioctrl_mlcd35rt_clear(); 
		return 1;

	case IOC_LCD35RT_MHLINE:
		_ioctrl_mlcd35rt_mhvline((struct lcd35rt_line *) arg); 
		return 1;

	case IOC_LCD35RT_MVLINE:
		ioctrl_mssd2119(0, IOC_SSD2119_VViHi, NULL);	
		_ioctrl_mlcd35rt_mhvline((struct lcd35rt_line *) arg); 
		lcd_conf.restore = RES_LCDD_ADDRC;
		return 1;

	case IOC_LCD35RT_MBOXFL:
		_ioctrl_mlcd35rt_mboxfull((struct lcd35rt_boxf *) arg);
		return 1;
	
	case IOC_LCD35RT_565BOX:
		_ioctrl_mlcd35rt_565box((lcd35rt_565shape_t *) arg);
		return 1;
	}
	return -1;
}

/*
 * ## ModuleInit_mlcd35rt()
 *

 */

void lcd35rt_dma_default_config(void) {
	uint32_t conf_fifo;

	
	/* default DMA configuration */
	/* DMA CONF { */
	lcd_conf.dma_CR = 0;
	lcd_conf.dma_CR = 	DMA_PeripheralDataSize_HalfWord | \
						DMA_MemoryDataSize_HalfWord | \
						DMA_Priority_Medium | \
						DMA_DIR_MemoryToMemory | \
						DMA_IT_TC;
	lcd_conf.dma_CR = ioctrl_mdma2(LCD35RT_OnDMA, IOC_DMA_CTRL, &lcd_conf.dma_CR);
	
	/* DMA fifo configuration */
	conf_fifo = 1;
	ioctrl_mdma2(LCD35RT_OnDMA, IOC_DMA_CTRL_FIFO, &conf_fifo);
	
	/* memory addresses for DMA */
	lcd_conf.addr.per_src = (uint32_t) &lcd_conf.color;	
	lcd_conf.addr.dest = (uint32_t) SSD2119_Data;
	lcd_conf.addr.size = 0;
	ioctrl_mdma2(LCD35RT_OnDMA, IOC_DMA_BIND, &lcd_conf.addr);
	/* } */

	/* LCD DRIVER CONF { */
	/* Set Window as full screen */
	lcd_conf.win.pos = 0;
	lcd_conf.win.height = LCD_VERTICAL_MAX;
	lcd_conf.win.width = LCD_HORIZONTAL_MAX;
	ioctrl_mssd2119(0, IOC_SSD2119_HWin, &lcd_conf.win);
	/* } */
	lcd_conf.dma_busy = 0;
}

int8_t ModuleInit_mlcd35rt() {
	// uint8_t i;

	// if(mdev_init(MAX_LCD35RT_MINORS, devs_lcd35rt) < 0)
	// 	return -1;
	/*
	for (i = 0; i < MAX_LCD35RT_MINORS; i++) {
		// Initalize mods[i]' fields
	}
	*/
	STM32f4_Discovery_LCD_Init();
	ModuleInit_mdma2();

	lcd35rt_dma_default_config();

	seek_mssd2119(0, 0, SEEK_SET);
	lcd_conf.restore = 0; 
	
	return 0;
}

/* Interrupt Handler */
void restore_conf(void) {

	if (lcd_conf.restore & RES_DMA_CR)
		ioctrl_mdma2(LCD35RT_OnDMA, IOC_DMA_CTRL, &lcd_conf.dma_CR);

	if (lcd_conf.restore & RES_DMA_ADDR) {
		lcd_conf.addr.per_src = (uint32_t) &lcd_conf.color;
		lcd_conf.addr.size = 0;
		ioctrl_mdma2(LCD35RT_OnDMA, IOC_DMA_BIND, &lcd_conf.addr);
	}

	if (lcd_conf.restore & RES_LCDD_WIN)
		ioctrl_mssd2119(0, IOC_SSD2119_HWin, &lcd_conf.win);
	
	if (lcd_conf.restore & RES_LCDD_ADDRC)
		ioctrl_mssd2119(0, IOC_SSD2119_HViHi, NULL);	

	lcd_conf.restore = 0;
}

void __IRQ DMA2_Stream0_IRQHandler(void) {
	if(DMA2->LISR & DMA_LISR_TCIF0)
		DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	
	if(lcd_conf.restore) restore_conf();
	
	lcd_conf.dma_busy = 0;
}


#endif /* _MDEV_USE_LCD35RT_*/
