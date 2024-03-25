/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		uart
 * 
 * File generated on 2024-03-05 14:56:18.859965
 * With mdev generator version 0.1.5 from 2024-03-05 13:16:39.358976
 ********************************************************************/

#ifdef _MDEV_USE_UART_


#ifndef UART_H
#define UART_H

#ifdef _MDEV_USE_LCD35RT_
	#define MAX_UART_MINORS 	1	// Pins are taken for the LCD
#else
	#define MAX_UART_MINORS 	2
#endif

#define IOC_UART_BAUDRATE 		1
// #define ioctrl_muart(mi, cmd, arg) _ioctrl_muart(mi, arg, cmd)

int8_t ModuleInit_muart();


/*
 * List of operators by default, some devices might need them all.
 * */

// int32_t open_muart(uint8_t min);
// int32_t close_muart(uint8_t min);
// int32_t read_muart(uint8_t min, void *buf, uint16_t size);
// int32_t write_muart(uint8_t min, void *buf, uint16_t size);
// int32_t _ioctrl_muart(uint8_t min, void *arg, uint16_t cmd);
// void seek_muart(uint8_t min, uint32_t offset, uint8_t whence);

#endif /*UART_H*/



#endif /* _MDEV_USE_UART_*/
