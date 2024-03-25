#ifndef _SDIO_CMD_H_
#define _SDIO_CMD_H_

/* This header is based on the Specs for SDIO:
 * 		PartE1_SDIO_Simplified_Specification_Ver3.00.pdf
 *
 */

/* List of commands according to specs */
#define 	SDIO_CMD0 		0	// IDLE
#define 	SDIO_CMD2 		2	// CID Register
#define 	SDIO_CMD3 		3	// RCA Register
#define 	SDIO_CMD5 		5	// OCR, to use SDIO
#define 	SDIO_CMD7		7	// OCR, to use SDIO
#define 	SDIO_CMD8 		8	// SDHC support
#define 	SDIO_CMD8 		8	// SDHC support
#define 	SDIO_CMD9 		9	// CSD Register
#define 	SDIO_CMD17  	17 	// Single block Read
#define 	SDIO_CMD18 		18 	// Multi block read
#define 	SDIO_CMD24		24 	// Single block write
#define 	SDIO_CMD25 		25 	// Multi block write
#define 	SDIO_CMD55 		55 	// Next command is gonna be Application Specific
#define 	SDIO_CMD58		58	// OCR, to use SPI
#define 	SDIO_ACMD41		41	//

/* List of recommended arguments */
#define 	SDIO_ARG0 		0x0
#define 	SDIO_ARG8 		0x1aa
#define 	SDIO_ARG55 		0x0
#define 	SDIO_ARG58 		0x0
#define 	SDIO_AARG41_HC 	0x40000000

/* R1 Response */
#define 	SDIO_R1_IDLE 	0x01000000
#define 	SDIO_R1_ILLEGAL 0x04000000
#define 	SDIO_R1_CRC_ERR 0x08000000
#define 	SDIO_R1_FN_ERR	0x10000000
#define		SDIO_R1_PAR_ERR	0x40000000

#define 	SDIO_SDHC_V2_0 	4

#endif
