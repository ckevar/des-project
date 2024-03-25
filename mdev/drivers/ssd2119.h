/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		ssd2119 chip
 * 
 * This supports the stm32f4_discovery_lcd.* 
 * 
 * File generated on 2024-02-16 09:37:32.991271
 * With mdev generator version 0.1.3 from 2024-02-15 14:43:40.628882
 ********************************************************************/

#ifdef _MDEV_USE_SSD2119_


#ifndef SSD2119_H
#define SSD2119_H

/* The same addresses are referenced on stm32f4_discovery_lcd.c but they are private */
#define SSD2119_BASE_Data                ((u32)(0x60000000|0x00100000))
#define SSD2119_BASE_Addr                ((u32)(0x60000000|0x00000000))
#define SSD2119_CMD                      ((vu16 *)SSD2119_BASE_Addr)
#define SSD2119_Data                     ((vu16 *)SSD2119_BASE_Data)
/* End of stm32f4_discover_lcd.c */

#define SSD2119_MINORS 					1	// [Edit] to include more minors
#define ssd2119_MKPOS(x, y) 			(((y) << 16) | (x))
#define ssd2119_XPOS(xy) 				(xy & 0xff)
#define ssd2119_YPOS(xy) 				(xy >> 16)
#define ssd2119_RGB565(r,g,b) 			(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))
#define ssd2119_hRGB565(rgb) 			(((rgb & 0xf80000) >> 8) | ((rgb & 0x00fc00) >> 5) | ((rgb & 0xf8) >> 3)) 

/* IO Controls { */
// 		definitions {
#define IOC_SSD2119_HViHi 				0
#define IOC_SSD2119_VViHi 				1
#define IOC_SSD2119_HWin 				2
// 		}
// 		structs {
struct ssd2119_window {
	uint32_t pos;
	uint16_t height;
	uint16_t width;
};
// 		}
/* } END IO Controls */

#define ioctrl_mssd2119(mi, cmd, arg) _ioctrl_mssd2119(mi, arg, cmd)
/* seek { */
//	structs {

// }
/* } end of seek*/
int8_t ModuleInit_mssd2119();


/*
 * List of operators by default, some devices might need them all.
 * */

// int32_t open_mssd2119(uint8_t min);
// int32_t close_mssd2119(uint8_t min);
// int32_t read_mssd2119(uint8_t min, void *buf, uint16_t size);
int32_t write_mssd2119(uint8_t min, void *buf, uint16_t size);
int32_t _ioctrl_mssd2119(uint8_t min, void *arg, uint16_t cmd);
void seek_mssd2119(uint8_t min, uint32_t offset, int16_t whence);

#endif /*SSD2119_H*/



#endif /* _MDEV_USE_SSD2119_*/
