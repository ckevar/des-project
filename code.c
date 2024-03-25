#include "ee.h"

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lcd.h"

// #include "app.h"
// #include "Widget.h"
// #include "WidgetConfig.h"

#include <stdint.h>
#include "mdev.h"
#include "drivers/leds.h"
#include "drivers/lcd35rt.h"
#include "drivers/ssd2119.h"
#include "mfat32/fat32.h"
#include "mnet.h"
#include "mnet/ssocket.h"

//#include "disdain_into_046_0.h"
#include "_tmp_test.h"
#include "lcd_log.h"

/*
 * SysTick ISR2
 */
ISR2(systick_handler) {
	CounterTick(myCounter);
}

TASK(Task_ledblue) {
	ioctrl_mleds(mLED_GREEN, IOC_LEDS_TOGGLE, NULL);
}

// Enable for LCD Touch
TASK(TaskLCDTouch) {
// 	unsigned int px, py;
// 	TPoint p;
	// if(GetTouch_SC_Async(&px, &py)) {
// 		p.x = px;
// 		p.y = py;
}

// }

/***************************************************************************
 * TASK Task_ESP82266_RevResponse
 * 
 * 	This search for specific words in the message received, but it looks for
 * 	them, starting in the begging of the buffer until where there's no more
 * 	chars, and everytime this task is executed, the RX_READ starts where the
 *  last search ended in the buffer.
 * 
 **************************************************************************/
TASK(Task_wifi_service) {
	/* wifi_driver state */
	// LCD_UsrLog("test\r\n");
	// LCD_UsrLog("%s", ESP8266_Buff.RX_READ);
	// if (ESP8266_AT_STATUS == ESP8266_ST_UNKNOWN_CODE) {
		// wifi_service();
	// }
	// STM_EVAL_LEDToggle(LED4);

}

/**
 * Task used for FSM.
 */
TASK(Task_ESP8266_FSM)
{
	// app_fsm_app();
}


/*
 * ESP8266 TRANSMITTER
 */
// void __IRQ EE_CORTEX_MX_DMA2_STREAM6_ISR(void) {
// 	if (DMA2->HISR & DMA_HISR_TCIF6) {
// 		// If interruption is because transfer is completed
// 		DMA2->HIFCR = DMA_HIFCR_CTCIF6;	// Ack interruptions
// 	} else if (DMA2->HISR & DMA_HISR_DMEIF6) {
// 		// If interruption on Direct Mode went wrong
// 		LCD_UsrLog("ERROR on sending\r\n");
// 		DMA2->HIFCR = DMA_HIFCR_CDMEIF6;	// Ack interruption
// 	}
// }

/*
 * ESP8266 RECEIVER
 */
void __IRQ EE_CORTEX_MX_DMA2_STREAM1_ISR(void) {
	// LCD_UsrLog("CR:%d\r\n", DMA2_Stream1->CR);
	if (DMA2->LISR & DMA_LISR_TCIF1) { 	// Interruption if reception is
										// completed.
		DMA2->LIFCR = DMA_LIFCR_CTCIF1;	// Ack interruptions
	} 
	else if (DMA2->LISR & DMA_LISR_TEIF1) {
		// If interruption on transfer went wrong
		DMA2->LIFCR = DMA_LIFCR_CTEIF1;	// Ack interruption
		// LCD_UsrLog("ERROR transfer\r\n");
	}
	else if (DMA2->LISR & DMA_LISR_DMEIF1) {
		// If interruption on Direct Mode went wrong
		DMA2->LIFCR = DMA_LIFCR_CDMEIF1;	// Ack interruption
		// LCD_UsrLog("ERROR Direct Mode\r\n");
	}
}

int main(void)
{
	/*
	 * Setup the microcontroller system.
	 * Initialize the Embedded Flash Interface, the PLL and update the
	 * SystemFrequency variable.
	 * For default settings look at:
	 * pkg/mcu/st_stm32_stm32f4xx/src/system_stm32f4xx.c
	 */
	SystemInit();

	/*Initialize Erika related stuffs*/
	EE_system_init();

	/*Initialize systick */
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
	EE_systick_enable_int();
	EE_systick_start();

	// DEBUG ECC
	mdev_core_init();
	ModuleInit_mleds();
	ModuleInit_mlcd35rt();
	// ModuleInit_mfat32();
	ModuleInit_mnet();

	bind_socks(21);
	uint8_t lid = listen_socks();
	uint8_t buf[512];
	read_socks(lid, buf, 512);

	// ioctrl_mnet(IOC_MNET_STAAP, &wpa_ap_credentials);

	lcd35rt_565shape_t hello_world565 = {
		.win = {
			.pos = 0,
			.height = _tmp_test[0],
			.width = _tmp_test[1],
		},
		.data = (uint32_t)(_tmp_test + 2),
	};


	ioctrl_mlcd35rt(0, IOC_LCD35RT_565BOX, &hello_world565);

	// sockedfd = msocket(AF_INET);
	// socketlcd = msocket(AF_UNIX);

	// write_term(STDERR, "HOLA", 4)
	// ioctrl_term(0, dim, sizeof(dim_t));

	// STDOUT = open(MOD_LCD35RT(0));
	// write(STDOUT, "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.\r\n", 576);
	// ledred = open(MOD_LEDS(1));
	// ledblue = open(MOD_LEDS(2));

	// write(ledblue, LED_ON, 1);
	// write(ledred, LED_ON, 1);

	SetRelAlarm(Alarm_ledblue, 1, 500);


	// END DEBUG ECC

	// init_system_peripherals();

	// LCD_debug_start();

	// app_init();

	// SetRelAlarm(Alarm_wifi_service, 1, 40);

	// SetRelAlarm(AlarmTaskLCDTouch, 1, 50);
	// SetRelAlarm(Alarm_ESP8266_FSM, 15, 80);
	// SetRelAlarm(Alarm_Weather_Update, 180000, 180000);

	for (;;) {
		uint32_t mem;
		mem	= DMA2_Stream0->PAR;
	}

}
