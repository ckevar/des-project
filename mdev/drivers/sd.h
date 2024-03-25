/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		sd
 * 
 * File generated on 2024-02-24 10:55:42.558736
 * With mdev generator version 0.1.4 from 2024-02-21 08:41:07.842164
 ********************************************************************/

// #ifdef _MDEV_USE_SD_


#ifndef SD_H
#define SD_H

#define SD_MINORS 				1			// [Edit] to include more minors
#define SDHC_BLOCK_SIZE 		512			// Size fixed for SDHC spec

enum {
	IOC_SD_MEM_INIT, 
	IOC_SD_POWER_ON,
	IOC_SD_DE_SELECT,
};

typedef enum {
	SD_OK = 0,
	SD_ERR_CMD0_TIMEOUT, 
	SD_ERR_UNUSABLE,
	SD_ERR_CMD8_TIMEOUT,
	SD_MEM_ALREADY_INITIALIZED,
	SD_ERR_CMD55_TIMEOUT,
	SD_ERR_CMD55_R1,
	SD_ERR_ACMD41_INITIALIZING,
	SD_ERR_ACMD41_TIMEOUT,
	SD_ERR_CMD2_TIMEOUT,
	SD_ERR_CMD3_TIMEOUT,
	SD_ERR_CMD7_TIMEOUT,
	SD_ERR_CMD9_TIMEOUT,
	SD_ERR_CSD_NOT_SUPPORTED,
	SD_ERR_CMD_SEND_RESP,
	SD_ERR_CMD_SEND_CMD,
} SDStatusT;

#define ioctrl_msd(mi, cmd, arg) _ioctrl_msd(mi, arg, cmd)

int8_t ModuleInit_msd();


/*
 * List of operators by default, some devices might need them all.
 * */

// int32_t open_msd(uint8_t min);
// int32_t close_msd(uint8_t min);
int32_t read_msd(uint8_t min, uint32_t *buf, uint16_t size);
int32_t write_msd(uint8_t min, uint32_t *buf, uint16_t size);
int32_t _ioctrl_msd(uint8_t min, void *arg, uint16_t cmd);
void seek_msd(uint8_t min, uint32_t offset, uint8_t whence);

#endif /*SD_H*/



// #endif  _MDEV_USE_SD_
