/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		ssd2119
 * 
 * File generated on 2024-02-16 09:37:32.991401
 * With mdev generator version 0.1.3 from 2024-02-15 14:43:40.628882
 ********************************************************************/

#ifdef _MDEV_USE_SSD2119_

#include "mdev.h"
#include "drivers/ssd2119.h"
#include "stm32f4_discovery_lcd.h"

#define _write_ssd2119(reg, val) 	*SSD2119_CMD = reg; *SSD2119_Data = val
#define SSD2119_MEM_SIZE 			ssd2119_MKPOS(LCD_HORIZONTAL_MAX, LCD_VERTICAL_MAX)

struct ssd2119 {
	// ssd2119's data
	uint32_t cursor; 
} ssd2119s;

/*
static struct ssd2119 ssd2119s[MAX_SSD2119_MINORS];
struct mdev devs_ssd2119[MAX_SSD2119_MINORS];
*/

void seek_mssd2119_update_pos();

/*
 * ## open_mssd2119(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t open_mssd2119(uint8_t min) {
	return 1;
}
*/

/*
 * ## close_mssd2119(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t close_mssd2119(uint8_t min) {
	return 1;
}
*/

/*
 * ## read_mssd2119(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t read_mssd2119(uint8_t min, void *buf, uint16_t size) {
	return 1;
}
*/

/*
 * ## write_mssd2119(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */

int32_t write_mssd2119(uint8_t min, void *buf, uint16_t size) {
	uint16_t *data = (uint16_t *) buf;
	for (int i = 0; i < size; ++i)
		*SSD2119_Data = data[i];
}

/*
 * ## _ioctrl_mssd2119(uint8_t min, void *arg, uint16_t cmd)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
void _ioctrl_mssd2119_HWin(struct ssd2119_window *win) {
	uint16_t vram, hsram, heram;

	vram = ssd2119_YPOS(win->pos);
	vram |= (vram + win->height - 1) << 8;
	_write_ssd2119(SSD2119_V_RAM_POS_REG, vram);

	hsram = ssd2119_XPOS(win->pos);
	_write_ssd2119(SSD2119_H_RAM_START_REG, hsram);

	heram = hsram + win->width - 1;
 	_write_ssd2119(SSD2119_H_RAM_END_REG, heram);

 	ssd2119s.cursor = win->pos;
 	seek_mssd2119_update_pos();
}

int32_t _ioctrl_mssd2119(uint8_t min, void *arg, uint16_t cmd) {
	switch(cmd){
	case IOC_SSD2119_HViHi:
		_write_ssd2119(SSD2119_ENTRY_MODE_REG, 0x6C30); // 0x6830
		return 1;

	case IOC_SSD2119_VViHi:
		_write_ssd2119(SSD2119_ENTRY_MODE_REG, 0x6C38); // 0x6838
		return 1;

	case IOC_SSD2119_HWin:
		_write_ssd2119(SSD2119_ENTRY_MODE_REG, 0x6C30); // 0x6830
		_ioctrl_mssd2119_HWin((struct ssd2119_window *)arg);
		return 1;
	}
	return -1;
}
/*
 * ## seek_mssd2119(uint8_t min, struct ssd2119_pos *offset, int16_t whence)
 *
 */

void seek_mssd2119_update_pos() {
	_write_ssd2119(SSD2119_X_RAM_ADDR_REG, ssd2119_XPOS(ssd2119s.cursor));
	_write_ssd2119(SSD2119_Y_RAM_ADDR_REG, ssd2119_YPOS(ssd2119s.cursor));
	*SSD2119_CMD = SSD2119_RAM_DATA_REG;
	// _write_ssd2119(SSD2119_RAM_DATA_REG, 0x0000);
}

void seek_mssd2119(uint8_t min, uint32_t offset, int16_t whence) {
	mdev_seek(&ssd2119s.cursor, offset, SSD2119_MEM_SIZE, whence);
	seek_mssd2119_update_pos();
}

/*
 * ## ModuleInit_mssd2119()
 * This module has already been started at stm32f4_discovery_lcd.c
 */
int8_t ModuleInit_mssd2119() {
	// uint8_t i;

	// if(mdev_init(MAX_SSD2119_MINORS, devs_ssd2119) < 0)
	// 	return -1;
	/*
	for (i = 0; i < MAX_SSD2119_MINORS; i++) {
		// Initalize mods[i]' fields
	}
	*/
	ssd2119s.cursor = 0;
	return 0;
}



#endif /* _MDEV_USE_SSD2119_*/
