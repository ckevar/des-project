/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		lcd35rt
 * 
 * File generated on 2024-02-15 14:43:44.983347
 * With mdev generator version 0.1.3 from 2024-02-15 14:43:40.628882
 ********************************************************************/

#ifdef _MDEV_USE_LCD35RT_


#ifndef LCD35RT_H
#define LCD35RT_H

#include "ssd2119.h"

#define LCD35RT_MINORS 					1	// [Edit] to include more minors

#define IOC_LCD35RT_CLEAR 				0
#define IOC_LCD35RT_MHLINE 				1
#define IOC_LCD35RT_MVLINE 				2
#define IOC_LCD35RT_MBOXFL 				3
#define IOC_LCD35RT_565BOX 				4

#define ioctrl_mlcd35rt(mi, cmd, arg) 	_ioctrl_mlcd35rt(mi, arg, cmd)

/* Monochromatic types and structs */
struct lcd35rt_pixel {
	uint16_t color;
	uint32_t pos;
};

struct lcd35rt_line {
	struct lcd35rt_pixel origin;
	uint16_t length;
};

struct lcd35rt_boxf {
	struct lcd35rt_pixel origin;
	uint16_t height;
	uint16_t width;
};

typedef union {
	struct lcd35rt_boxf box;
	struct lcd35rt_line line;
}  lcd35rt_mono_shapes_t;
/* END Monochromatic types and structs */

/*  RGB565 types and structs */
typedef struct {
	struct ssd2119_window win;
	uint32_t data;
} lcd35rt_565shape_t;
/* END RGB565 types and structs */

int8_t ModuleInit_mlcd35rt();

/*
 * List of operators by default, some devices might need them all.
 * */

// int32_t open_mlcd35rt(uint8_t min);
// int32_t close_mlcd35rt(uint8_t min);
// int32_t read_mlcd35rt(uint8_t min, void *buf, uint16_t size);
// int32_t write_mlcd35rt(uint8_t min, void *buf, uint16_t size);
int32_t _ioctrl_mlcd35rt(uint8_t min, void *arg, uint16_t cmd);

#endif /*LCD35RT_H*/



#endif /* _MDEV_USE_LCD35RT_*/
