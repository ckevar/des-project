/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		leds
 * 
 * File generated on 2024-02-15 13:30:08.353523
 * With mdev generator version 0.1.2 from 2024-02-15 13:29:56.956766
 ********************************************************************/

#ifdef _MDEV_USE_LEDS_


#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

#ifdef _MDEV_USE_LCD35RT_
	#define LEDS_MINORS 1 	// Orange, Blue and Red Led are used for the LCD
	#define mLED_GREEN 		0
#else
	#define LEDS_MINORS 4
	#define mLED_ORANGE 	0
	#define mLED_GREEN 		1
	#define mLED_RED 		2
	#define mLED_BLUE 		3

#endif /* _MDEV_USE_LCD35RT_ */

#define IOC_LEDS_ON  	0
#define IOC_LEDS_OFF 	1
#define IOC_LEDS_TOGGLE 2
#define IOC_LEDS_CTRLS 	3
#define ioctrl_mleds(mi, cmd, arg) _ioctrl_mleds(mi, arg, cmd)

int8_t ModuleInit_mleds();

int32_t _ioctrl_mleds(uint8_t min, void *arg, uint16_t cmd);

#endif /*LEDS_H*/



#endif /* _MDEV_USE_LEDS_*/
