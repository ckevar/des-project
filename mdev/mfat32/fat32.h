/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		fat32
 * 
 * File generated on 2024-02-21 18:33:05.419438
 * With mdev generator version 0.1.4 from 2024-02-21 08:41:07.842164
 ********************************************************************/


#ifndef FAT32_H
#define FAT32_H
#include <stdint.h>

#define MAX_FAT32_MINORS 	4	// There are at most 4 records per partion
								// in a single microsd, according to the specs

// #define ioctrl_mfat32(mi, cmd, arg) _ioctrl_mfat32(mi, arg, cmd)

int8_t ModuleInit_mfat32();


/*
 * List of operators by default, some devices might need them all.
 * */

uint32_t open_mfat32(const char *min);
// int32_t close_mfat32(uint8_t min);
uint32_t read_mfat32(uint32_t fa, uint8_t *buf, uint32_t size);
// int32_t write_mfat32(uint8_t min, void *buf, uint16_t size);
// int32_t _ioctrl_mfat32(uint8_t min, void *arg, uint16_t cmd);
// void seek_mfat32(uint8_t min, uint32_t offset, uint8_t whence);

#endif /*FAT32_H*/



