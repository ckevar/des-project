/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		net
 * 
 * File generated on 2024-03-05 14:48:11.399653
 * With mdev generator version 0.1.5 from 2024-03-05 13:16:39.358976
 ********************************************************************/

#ifdef _MDEV_USE_NET_


#ifndef NET_H
#define NET_H

#include <stdint.h>

#define MAX_NET_MINORS 		1	// There's only one network dev

#define BIND_PORT			1
#define LISTEN 				2

#define ioctrl_mnet(cmd, arg) _ioctrl_mnet(arg, cmd)

int8_t ModuleInit_mnet();


/*
 * List of operators by default, some devices might need them all.
 * */

// int32_t open_mnet(uint8_t min);
// int32_t close_mnet(uint8_t min);
uint16_t read_mnet(uint8_t min, uint8_t *buf, uint16_t size);
// int32_t write_mnet(uint8_t min, void *buf, uint16_t size);
uint8_t _ioctrl_mnet(void *arg, uint16_t cmd);
// void seek_mnet(uint8_t min, uint32_t offset, uint8_t whence);

#endif /*NET_H*/



#endif /* _MDEV_USE_NET_*/
