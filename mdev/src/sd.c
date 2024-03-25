/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		sd
 * 
 * 
 * Resources:
 *  - 	Part1 Physical Layer Simplified Specifications ver6.00
 *
 *  - 	SD Specifications Part E1 SDIO Simplified Specification v3.00: 
 * 		PartE1_SDIO_Simplified_Specification_Ver3.00.pdf
 * 
 *  - 	SD Card Initiaization Part 1-4 for SPI
 * 		http://www.rjhcoding.com/avrc-sd-interface-1.php
 * 		
 * 	- 	RM0090 Reference manual: STM32F405/415, STM32F407/417, STM32F427/437 and
 *		STM32F429/439 advanced Arm ® -based 32-bit MCUs
 *
 *  File generated on 2024-02-24 10:55:42.558840
 * With mdev generator version 0.1.4 from 2024-02-21 08:41:07.842164
 ********************************************************************/

// #ifdef _MDEV_USE_SD_


#include "stm32f4_discovery.h"

#include "mdev.h"
#include "drivers/sd.h"
#include "sd/sdio/cmd.h"
#include "sd/sdio/std.h"
#include "sd/stm32f4x/constants.h"

#define SDIO_CMDx_ATTEMPTS 		255

#define SDIO_CMD_SEND(cmd, arg) SDIO->ICR = SDIO_CLR_STA; \
								SDIO->ARG = arg; \
								SDIO->CMD = (SDIO_CMD_MSK & (cmd | SDIO_CPSM_Enable))

#define	WAIT_STA_TIL(guard)		while ((SDIO->STA & (guard)) == 0)

#define SDIO_RDWR_DGUARDS 		(SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | \
								SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR)

#define SDIO_READ_DGUARDS  		(SDIO_RDWR_DGUARDS | SDIO_FLAG_RXOVERR)
#define SDIO_WRITE_DGUARDS 		(SDIO_RDWR_DGUARDS | SDIO_FLAG_TXUNDERR)

#define SDIO_CMD_GUARDS 		(SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND | \
								SDIO_FLAG_CCRCFAIL)


struct sd {
	// sd's data
	SDStatusT status;
	uint8_t flags;
	volatile uint32_t bkcursor; 	// cursor block
	union CSDv2 csd;
	uint32_t rca;
	uint32_t capacityKB; 		// SD Capacity in KB
	uint32_t capacityBL; 		// SD Capacity in Blocks (512 bytes each)
};

static struct sd sds;

/*
static struct sd sds [MAX_SD_MINORS];
*/



/*
 * ## open_msd(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t open_msd(uint8_t min) {
	return 1;
}
*/

/*
 * ## close_msd(uint8_t min)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */
/*
int32_t close_msd(uint8_t min) {
	return 1;
}
*/

/*
 * ## read_msd(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */

void _read_msd_cmd_RDWR(uint32_t cmd) {
	
	SDIO_CMD_SEND(cmd | SDIO_Response_Short, sds.bkcursor);

	WAIT_STA_TIL(SDIO_CMD_GUARDS);

	if(((SDIO->RESP1 & SDIO_R1_MSK) > 0)) 
		sds.status = SD_ERR_CMD_SEND_RESP;

	SDIO->ICR = SDIO_CLR_STA;
	/*
	if(SDIO->RESPCMD != cmd) {
		sds.status = SD_ERR_CMD_SEND_CMD;
		return;
	}
	*/
}

int32_t read_msd(uint8_t min, uint32_t *buf, uint16_t size) {

	uint32_t tmp_u32;
	uint8_t i;

	// TODO
	// Check in the flags if memory got initialized

	SDIO->DLEN = size * 512;
	SDIO->DCTRL |= SDIO_TransferDir_ToSDIO | SDIO_DPSM_Enable;
	
	/* Tell SD this is a reading */
	tmp_u32 = SDIO->DLEN > 512 
			  ? SDIO_CMD18 
			  : SDIO_CMD17;
	
	_read_msd_cmd_RDWR(tmp_u32);
	
	if(sds.status != SD_OK)
		return -1;

	/* wait for reading package */
	WAIT_STA_TIL(SDIO_READ_DGUARDS) {
		if(SDIO->STA & (SDIO_FLAG_RXFIFOHF | SDIO_FLAG_RXOVERR)) {
			for (i = 0; i < 8; i ++)
				*(buf + i) = SDIO->FIFO;
			buf += 8;
		}
	}

	if (SDIO->STA & SDIO_FLAG_DBCKEND)
		return size;
	
/* 	else { 
 * 	// return the error */
		return (-1 | (SDIO->STA & 0x00FFFDFF));
/* 	} */
}


/*
 * ## write_msd(uint8_t min, void *buf, uint16_t size)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */

int32_t write_msd(uint8_t min, uint32_t *buf, uint16_t size) {
	uint32_t tmp_u32;
	uint8_t i, j;

	// TODO:
	// 1. Check in Flags in memory got initialized.
	
	tmp_u32 = size > 1
			  ? SDIO_CMD25
			  : SDIO_CMD24;

	_read_msd_cmd_RDWR(tmp_u32);

	if (sds.status != SD_OK)
		return -1;
	
	SDIO->DLEN = size * 512;
	SDIO->DCTRL = (SDIO->DCTRL & ~SDIO_TransferDir_ToSDIO) | SDIO_DPSM_Enable;
	
	for(i = 0; i < SDIO->DLEN / 4; i++) {
		SDIO->FIFO = *(buf + i);
		while(SDIO->STA & SDIO_FLAG_TXFIFOF);
	}


	/*
	j = 32;
	do {
		for (i = 0; i < j; i++)
			SDIO->FIFO = *(buf + i);
		buf += j;
		while(SDIO->STA & SDIO_FLAG_TXFIFOHE == 0);
		j = 9;
	} WAIT_STA_TIL(SDIO_WRITE_DGUARDS);
	*/

	if(SDIO->STA & SDIO_FLAG_DBCKEND)
		return size;

	return (-1 | (SDIO->STA &0x00FFFDFF));
}


/*
 * ## _ioctrl_msd(uint8_t min, void *arg, uint16_t cmd)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */

void _ioctrl_msd_idle(void) {
	SDIO_CMD_SEND(SDIO_CMD0, SDIO_ARG0);
	WAIT_STA_TIL(SDIO_FLAG_CMDSENT);
}

void _ioctrl_msd_hc(void) {	
	// Command with response R1
	uint8_t attempts;
	uint32_t attempt_mask;

	attempts = SDIO_CMDx_ATTEMPTS; 
	attempt_mask = SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT;

	do {

		SDIO_CMD_SEND(SDIO_CMD8 | SDIO_Response_Short, SDIO_ARG8);
		WAIT_STA_TIL(SDIO_CMD_GUARDS);
		attempts--;
	
	} while (attempts && (SDIO->STA & attempt_mask) && 
			 ((SDIO->RESP1 & SDIO_R1_MSK) < 0x2000000));

	if (SDIO->STA & SDIO_FLAG_CTIMEOUT) {
		sds.status = SD_ERR_CMD8_TIMEOUT;
		return;
	}

	if(((SDIO->RESP1 & SDIO_R1_MSK) < 0x2000000)) {
		sds.flags |= SDIO_F8_FLAG;	/* > SD V2 */
		return;
	}

	if(SDIO->RESP1 & SDIO_R1_ILLEGAL) {
		sds.flags &= ~SDIO_F8_FLAG;
		return;
	}
	
	sds.status = SD_ERR_UNUSABLE; 
}


void sd_app_cmd(void) {
	uint8_t attempts;
	uint32_t attempt_mask;
	uint16_t delay;

	attempts = SDIO_CMDx_ATTEMPTS;
	attempt_mask = SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT;
	delay = 50000;
 	
	do {

		SDIO_CMD_SEND(SDIO_CMD55 | SDIO_Response_Short, SDIO_ARG55);
		while(delay) delay--;
		WAIT_STA_TIL(SDIO_CMD_GUARDS);
		attempts--;
 	
	} while(attempts && SDIO->STA & attempt_mask);

 	if (SDIO->STA & SDIO_FLAG_CMDREND) { 		
		
		if(((SDIO->RESP1 & SDIO_R1_MSK) > 0x1000000)) 
			sds.status = SD_ERR_CMD55_R1;

		/*
		 * else { 
		 * 	idle state, it's ok
		 * } 
		 */
		return;
	}

 	sds.status = SD_ERR_CMD55_TIMEOUT;
}

void sd_op_cmd(uint32_t arg) {
 	
	/* broadcast command with response r3 / no CRC */
	SDIO_CMD_SEND(SDIO_ACMD41 | SDIO_Response_Short, arg);

	/* Dont check neither CRC nor CMDREND, because ACMD41 has an r3
	 * response, meaning, no CRC, if CRC fails then CMDREND is not set
	 * 	- STM32f4x: RM0090, page 1068.
	 */
	WAIT_STA_TIL(SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT);

	/* Since it's an r3 response, it yields CRC failure */
	if (SDIO->STA & SDIO_FLAG_CCRCFAIL) {
		if ((SDIO->RESP1 & SDIO_OCR_MRDY) == 0)
			sds.status = SD_ERR_ACMD41_INITIALIZING;
		
		else 
			sds.flags |= (SDIO->RESP1 & SDIO_OCR_S18); 
		
		return;
	}
	sds.status = SD_ERR_ACMD41_TIMEOUT;
}


void _ioctrl_msd_mem_init(uint32_t *support) {
	uint8_t attempts;

	if(sds.flags & SDIO_MEM_FLAG) {
		sds.status = SD_MEM_ALREADY_INITIALIZED;
		return;
	}

	attempts = SDIO_CMDx_ATTEMPTS;

	// do {

		sds.status = SD_OK;
		sd_app_cmd();
		
		if (sds.status != SD_OK)
			return;

		sd_op_cmd(*support);
		attempts--;
	
	//} while (attempts && (sds.status != SD_OK));
	
	if (sds.status != SD_OK)
		return;

	sds.flags |= SDIO_MEM_FLAG;
}

void msd_power_up(void) {	
	SDIO_DeInit();

	// 1-wire and 400kHz for configuration
	SDIO->CLKCR = SDIO_BusWide_1b | SDIO_CLK_400kHz;
	
	SDIO->POWER = SDIO_PowerState_ON;

	// Enable Clock
	SDIO->CLKCR |= 0x100;
}

void _ioctrl_msd_cid(void) {
	
	// Broadcast Command with response r2 / 127 bits + CRC
	SDIO_CMD_SEND(SDIO_CMD2 | SDIO_Response_Long, 0);
	WAIT_STA_TIL(SDIO_CMD_GUARDS);

	if(SDIO->STA & SDIO_FLAG_CTIMEOUT)
		sds.status = SD_ERR_CMD2_TIMEOUT;

/*
 * 	// <OPTIONAL>
 * 	else {
 * 		save locally CID
 * 	}
 */
}

void _ioctrl_msd_rca(void) {
	uint8_t attempts;
	attempts = SDIO_CMDx_ATTEMPTS;

	do {
		// Broadcast Command with response r6 / 32 bits + CRC
		SDIO_CMD_SEND(SDIO_CMD3 | SDIO_Response_Short, 0);
		WAIT_STA_TIL(SDIO_CMD_GUARDS);	
		attempts--;

	} while (attempts && (SDIO->STA & SDIO_FLAG_CCRCFAIL));

	if (SDIO->STA & SDIO_FLAG_CTIMEOUT) {
		sds.status = SD_ERR_CMD3_TIMEOUT;
		return;
	}
	
	sds.rca = SDIO->RESP1;
}

void _ioctrl_msd_csd(void) {
	
	// Broadcast Command with response r2 / 127 bits + CRC
	SDIO_CMD_SEND(SDIO_CMD9 | SDIO_Response_Long, sds.rca);
	WAIT_STA_TIL(SDIO_CMD_GUARDS);	
	
	if (SDIO->STA & SDIO_FLAG_CTIMEOUT) {
		sds.status = SD_ERR_CMD9_TIMEOUT;
		return;
	}
	
	sds.csd.raw[0] = SDIO->RESP1;
	sds.csd.raw[1] = SDIO->RESP2;
	sds.csd.raw[2] = SDIO->RESP3;
	sds.csd.raw[3] = SDIO->RESP4;

	/* Check CSD Version (not support for V1)*/
	if (sds.csd.csd.Structure != CSD2_STRUCTURE) {
		sds.status = SD_ERR_CSD_NOT_SUPPORTED;
		return;
	}

	/* Compute SD Capacity */
	sds.capacityKB = (sds.csd.csd.CSizeH & CSD2_C_SIZE_H) << 16;
	sds.capacityKB = (sds.capacityKB | sds.csd.csd.CSizeL + 1) * 512;
	sds.capacityBL = sds.capacityKB << 2;	
	
}

void _ioctrl_msd_power_on(void) {
	uint32_t support;
	/* sds.flags according to the SSD specs */
	sds.flags &= ~(SDIO_IO_FLAG | SDIO_MEM_FLAG | SDIO_F2_FLAG);
	sds.status = SD_OK;
	
	msd_power_up();

	/* CMD0 */
	_ioctrl_msd_idle();
	if(sds.status != SD_OK) 
		return;
	
	/* CMD8 */
	_ioctrl_msd_hc();
	if(sds.status != SD_OK)
		return;

	/* 
	 * CMD5:
	 * Most cards out there do not support I/O OCR
	 * so, skipped.
	 */
	
	/* 
	 * CMD55 + ACMD41:
	 * High Capacity support and due to the voltage of the 
	 * STM32f4x, the voltage supported is from 3.2v to 3.3v
	 */
	support = SDIO_AARG41_HC | SDIO_OCR_v32_33; 
	_ioctrl_msd_mem_init(&support);
	if(sds.status != SD_OK)
		return;

	/* 
	 * CMD11:
	 * UHS-I not supported, use it if:
	 * 		sds.flag & SDIO_OCR_S18 
	 */

	/* CMD2 */
	_ioctrl_msd_cid();
	if(sds.status != SD_OK)
		return;
	
	/* CMD3 */
	_ioctrl_msd_rca();
	if(sds.status != SD_OK)
		return;
	
	/* CMD9 */
	_ioctrl_msd_csd();
}

void _ioctrl_msd_de_select(void) {
	uint8_t attempts;
	
	attempts = SDIO_CMDx_ATTEMPTS;
	do {
		
		// Broadcast Command with response r1b / 32 bits + CRC
		SDIO_CMD_SEND(SDIO_CMD7 | SDIO_Response_Short, sds.rca);
		WAIT_STA_TIL(SDIO_CMD_GUARDS);
		attempts--;
	
	} while(attempts && (SDIO->STA & SDIO_FLAG_CCRCFAIL));

	if (SDIO->STA & SDIO_FLAG_CTIMEOUT)
		sds.status = SD_ERR_CMD7_TIMEOUT;
}

int32_t _ioctrl_msd(uint8_t min, void *arg, uint16_t cmd) {
	switch(cmd) {
	case IOC_SD_MEM_INIT:
		_ioctrl_msd_mem_init((uint32_t *)arg);
		return sds.status;

	case IOC_SD_POWER_ON:
		_ioctrl_msd_power_on();
		return sds.status;

	case IOC_SD_DE_SELECT:
		_ioctrl_msd_de_select();
		return sds.status;
	}
	return -1;
}


/*
 * ## seek_msd(uint8_t min, uint32_t offset, uint8_t whence)
 *
 * [Edit] Uncomment the function to use. This is the comment section, you
 *  can write comments here for documentation purposes.
 */

void seek_msd(uint8_t min, uint32_t offset, uint8_t whence) {
	//mdev_seek(&sds.bkcursor, offset, 0xffff, whence);
	if(whence == SEEK_SET) sds.bkcursor = offset;
}


/*
 * ## ModuleInit_msd()
 *
 */
void init_sd_forcely(void) {
	uint8_t attempt;
	attempt = SDIO_CMDx_ATTEMPTS;

	do {
		
		_ioctrl_msd_power_on();
		attempt--;

	} while(attempt && sds.status != SD_OK);
	
	if(sds.status) return;

	attempt = SDIO_CMDx_ATTEMPTS;
	do {
		
		_ioctrl_msd_de_select();
		attempt--;
	
	} while(attempt && sds.status != SD_OK);
}

int8_t ModuleInit_msd() {

	/* Inits GPIOs */
	SD_LowLevel_Init();

	/* Reset SDIO module */
	SDIO_DeInit();

	/* Basic SDIO Reg configs */
	SDIO->DTIMER = 0xFFFFFFFF;
	SDIO->DCTRL = SDIO_DataBlockSize_512b;

	init_sd_forcely();

	if (sds.status)
	   return sds.status;

	return 0;
}

void __IRQ SDIO_IRQHandler(void) {
	uint32_t tmp_u32;
	tmp_u32 = SDIO->STA;
}

void __IRQ EE_CORTEX_MX_NMI_ISR(void) {}
void __IRQ EE_CORTEX_MX_HARD_FAULT_ISR(void) {}
void __IRQ EE_CORTEX_MX_MPU_FAULT_ISR (void) {}
void __IRQ EE_CORTEX_MX_BUS_FAULT_ISR (void) {}
void __IRQ EE_CORTEX_MX_USAGE_FAULT_ISR (void) {}
