/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		leds
 * 
 * File generated on 2024-02-15 13:22:01.746568
 * With mdev generator version 0.1.1 from 2024-02-15 13:22:01.364619
 ********************************************************************/

#ifdef _MDEV_USE_LEDS_


// #include "mdev.h"
#include "drivers/leds.h"

#include "stm32f4_discovery.h"

#define LED_ACTIONS 3 // On, off, toggle

struct {
	void (*action)(uint8_t);
} leds_actions[LED_ACTIONS];

uint8_t leds_id[LEDS_MINORS];

int32_t _ioctrl_mleds(uint8_t min, void *arg, uint16_t cmd) {
	if (cmd > IOC_LEDS_CTRLS) return -1;
	(*leds_actions[cmd].action)(leds_id[min]);
	return 1;		
}


/*
 * ## ModuleInit_leds()
 *

 */
int8_t ModuleInit_mleds() {
	uint8_t i;

#ifdef _MDEV_USE_LCD35RT_	/* STM32f4 discovery, the leds are as followed */
	leds_id[0] = LED4; 		// green
#else
	leds_id[0] = LED3; 		// orange
	leds_id[1] = LED4;		// green 
	leds_id[2] = LED5; 		// red
	leds_id[3] = LED6;  	// blue
#endif

	for ( i = 0; i < LEDS_MINORS; i++)
		STM_EVAL_LEDInit(leds_id[i]);
	
	leds_actions[IOC_LEDS_ON].action = STM_EVAL_LEDOn;
	leds_actions[IOC_LEDS_OFF].action = STM_EVAL_LEDOff;
	leds_actions[IOC_LEDS_TOGGLE].action = STM_EVAL_LEDToggle;

	return 0;
}



#endif /* _MDEV_USE_LEDS_*/
