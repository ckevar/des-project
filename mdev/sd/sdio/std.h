#ifndef _SDIO_STD_H_
#define _SDIO_STD_H_

/* This header is based on the Specs for SDIO:
 * 		Part1_Physical_Layer_Simplified_specifications_ver6.00.pdf
 * 		PartE1_SDIO_Simplified_Specification_Ver3.00.pdf
 * 
 * ONLY SUPPORTS 32-bits data set microcontroller.
 */
struct CSDRegisterV2 {
	uint8_t 	TranSpeed;		// 
	uint16_t 	TaacNsac;		// TAAC, NSAC
	uint8_t 	Structure; 		// 
	uint8_t 	CSizeH;			// SD Capacity MSB
	uint8_t 	Misalign;		// READ_BL_PARTIAL, WRITE_BLK_MISALIGN,
								// READ_BLK_MISALIGN, DSR_IMP
	uint16_t 	CCCBlockLen;	// CCC,READ_BL_LEN
								// READ_BLK_MISALIGN, C_SIZE
	uint16_t 	WEBlock;		// WP_GRP_SIZE, SECTOR_SIZE, ERASE_BLK_EN
	uint16_t 	CSizeL; 		// SD Capacity LSB
	uint8_t 	CCRC;			// 
	uint8_t 	Protection;		// FILE_FORMAT_GRP, COPY, PERM_WRITE_PROTECT, 
								// TMP_WRITE_PROTECT, FILE_FORMAT
	uint16_t 	WWSizes;		// WP_GRP_ENABLE, R2W_FACTOR, WRITE_BL_LEN, 
								// WRITE_BL_PARTIAL 
		
} __attribute__((packed));

union CSDv2 {
	struct CSDRegisterV2 csd;
	uint32_t raw[4];
};
// CSD Register, relative to the struct CSDRegister
// 	CSDRegisterV2.TaacNsac {
#define CSD2_TAAC 				0xFF00
#define CSD2_NSAC 				0x00FF
//	}

//	CSDRegisterV2.Structure {
#define CSD2_STRUCTURE 			0x40
// 	}

// 	CSDRegisterV2.CSizeH {
#define CSD2_C_SIZE_H 			0x3F
// 	}

// 	CSDRegisterV2.Misalign {
#define CSD2_READ_BL_PARTIAL 	0x80
#define CSD2_WRITE_BLK_MISALIGN 0x40
#define CSD2_READ_BLK_MISALIGN 	0x20
#define CSD2_DSR_IMP 			0x10
// 	}

// 	CSDRegisterV2.CCCBlockLen {
#define CSD2_CCC 				0xFFF0
#define CSD2_READ_BL_LEN 		0x000F
// 	}

// 	CSDRegisterV2.WEBlock {
#define CSD2_ERASE_BLK_EN		0x4000
#define CSD2_SECTOR_SIZE 		0x3F80
#define CSD2_WP_GRP_SIZE		0x007F
// 	}

// 	CSDRegisterV2.CCRC {
#define CSD2_CRC 				0xFE
// 	}

// 	CSDRegisterV2.Protection {
#define CSD2_FILE_FORMAT 		0x0C
#define CSD2_TMP_WRITE_PROTECT 	0x10
#define CSD2_COPY 				0x40
#define CSD2_PERM_WRITE_PROTECT 0x20
#define CSD2_FILE_FORMAT_GRP 	0x80
// 	}

// 	CSDRegisterV2.WWSizes {
#define CSD2_WP_GRP_ENABLE		0x8000
#define CSD2_R2W_FACTOR			0x1C00
#define CSD2_WRITE_BL_LEN 		0x03C0
#define CSD2_WRITE_BL_PARTIAL  	0x0020
//	}

// CSDv2 Default Values
#define CSD2_STRUCTURE_v2 		0x40
#define CSD2_TAAC_NSAC_Val 		0x0E00
#define CSD2_NSAC_Val 			0x0000
#define CSD2_TRAN_SPEED25		0x32 	// 25Mbit/s
#define CSD2_TRAN_SPEED50		0x5A	// 50Mbit/s
#define CSD2_TRAN_SPEED100 		0x0B 	// 100Mbit/s
#define CSD2_TRAN_SPEED200		0x2B 	// 200Mbit/s
#define CSD2_CCC_Value 			0x5B50
#define CSD2_READ_BL_LEN_Value 	0x9
#define CSD2_SECTOR_SIZE_Val 	0x3F80
#define CSD2_WRITE_BL_LEN_Val	0x0240

// Power Up Flags
// 		LSB Flags
#define SDIO_IO_FLAG 		0b0001
#define SDIO_MEM_FLAG 		0b0010
#define SDIO_F8_FLAG 		0b0100
#define SDIO_F2_FLAG 		0b1000

// 		MSB Flags
#define SDIO_CMD5_IORDY		0x80000000
#define SDIO_CMD5_NF 		0x70000000
#define SDIO_CMD5_MP		0x08000000
#define SDIO_OCR_S18		0x01000000
#define SDIO_OCR_MRDY		0x80000000
#define SDIO_ACMD41_HCS 	0x40000000
#define SDIO_ACMD41_CCS 	0x40000000
// End Power Up Flags

#endif
