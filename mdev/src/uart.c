/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		uart
 * 
 * File generated on 2024-03-05 14:56:18.860074
 * With mdev generator version 0.1.5 from 2024-03-05 13:16:39.358976
 ********************************************************************/

#ifdef _MDEV_USE_UART_


#include "mdev.h"
#include "drivers/uart.h"
#include "stm32f4_discovery.h"

struct muart {
	// muart's data 
	COM_TypeDef comx;
};

static struct muart uarts[MAX_UART_MINORS];

extern USART_TypeDef* COM_USART[COMn]; 

/*
 * ## open_muart(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t open_muart(uint8_t min) {
	return 1;
}
*/

/*
 * ## close_muart(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t close_muart(uint8_t min) {
	return 1;
}
*/

/*
 * ## read_muart(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t read_muart(uint8_t min, void *buf, uint16_t size) {
	return 1;
}
*/

/*
 * ## write_muart(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t write_muart(uint8_t min, void *buf, uint16_t size) {
	return 1;
}
*/

/*
 * ## _ioctrl_muart(uint8_t min, void *arg, uint16_t cmd)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */

void _ioctrl_muart_baudrate(USART_TypeDef *USARTx, uint32_t *baudrate) {
	uint32_t tmpreg = 0x00, apbclock = 0x00;
	uint32_t integerdivider = 0x00;
    uint32_t fractionaldivider = 0x00;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	
	if ((USARTx == USART1) || (USARTx == USART6)) {
		apbclock = RCC_ClocksStatus.PCLK2_Frequency;
	} else {
		apbclock = RCC_ClocksStatus.PCLK1_Frequency;
	}
	    
	/* Determine the integer part */
    if ((USARTx->CR1 & USART_CR1_OVER8) != 0) {
		/* Integer part computing in case Oversampling mode is 8 Samples */
		integerdivider = ((25 * apbclock) / (2 * (*baudrate)));    
	} else /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */ {
		/* Integer part computing in case Oversampling mode is 16 Samples */
		integerdivider = ((25 * apbclock) / (4 * (*baudrate)));    
	}

	tmpreg = (integerdivider / 100) << 4;

	/* Determine the fractional part */
	fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

	/* Implement the fractional part in the register */
	if ((USARTx->CR1 & USART_CR1_OVER8) != 0) {
		tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
    } else /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */ {
		tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
	}
				    
	/* Write to USART BRR register */
	USARTx->BRR = (uint16_t)tmpreg;
}

int32_t _ioctrl_muart(uint8_t min, void *arg, uint16_t cmd) {
	switch(cmd) {
	case IOC_UART_BAUDRATE: 
		_ioctrl_muart_baudrate(COM_USART[uarts[min].comx], (uint32_t *) arg);
		break;
	}
	return 1;
}

/*
 * ## seek_muart(uint8_t min, uint32_t offset, uint8_t whence)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
void seek_muart(uint8_t min, uint32_t offset, uint8_t whence) {
	return 1;
}
*/

/*
 * ## ModuleInit_muart
 *
 */

int8_t ModuleInit_muart() {
	uint8_t i;
   	USART_InitTypeDef usart;                                            

	uarts[0].comx = COM1;

#ifndef _MDEV_USE_LCD35RT_
	/* Fill uarts */
#endif
	
	for (i = 0; i < MAX_UART_MINORS; i++) {
		usart.USART_BaudRate = 115273;
		usart.USART_WordLength = USART_WordLength_8b;
		usart.USART_StopBits = USART_StopBits_1;
		usart.USART_Parity = USART_Parity_No;
		usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		STM_EVAL_COMInit(uarts[0].comx, &usart);
	}

	return 0;
}



#endif /* _MDEV_USE_UART_*/
