#include <stdint.h>
#include <string.h>
#include "mdev.h"
#include "mfat32/format16GB.h"
#include "mfat32/fat32_t.h"
#include "drivers/sd.h"

/*
 * THIS IS A UTILITY ONLY
 * It does not fulfill the Specs at 100%: 
 * 1. 	It misses a copy of the bootsector in the Reserved Sector,
 * 2. 	It misses a copy of the second FAT
 */

struct {
	uint32_t ReservedSectors;
	uint32_t FirstDataSector;
	uint32_t SectorsPerCluster;
} params;

void format16GB_computeSectors(struct BootSector *BPB, uint32_t offset_bytes) {
	params.ReservedSectors = offset_bytes/SDHC_BLOCK_SIZE + BPB->ReservedSectors;
	params.FirstDataSector = params.ReservedSectors + (BPB->NumFATs * BPB->FATSz32);
	params.SectorsPerCluster = BPB->SectorsPerCluster;
}
uint8_t mf32_formatF32_SD(uint32_t lba) {
	union {
		struct BootSectorBlock parse;
		uint8_t raw[SDHC_BLOCK_SIZE];
	} buf = {
		.raw = {
		0xEB, 0x58, 0x90,
		'm', 'd', 'e', 'v', '.', 'f', 'a', 't',
		0x00, 0x02,
		64,
		0x20, 0x00,
		2,
		0x00, 0x00,
		0x00, 0x00,
		0xF8,
		0x00, 0x00,
		0x13, 0x00,
		0x04, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0xAC, 0xDA, 0x01,
		0x18, 0x20, 0x00, 0x00,
		0x00, 0x00,
		0x00, 0x00,
		0x02, 0x00, 0x00, 0x00,
		0x01, 0x00,
		0x06, 0x00, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0,
		0x29,
		0x12, 0x54, 0x03, 0x24,
		'm', 'd', 'e', 'v', 'f', 'o', 'r','m','a','t', 0,
		'F', 'A','T', '3', '2', ' ', ' ', ' ', 0, 0
		},
	};

	/* Write the BootSector */
	seek_msd(0, lba/SDHC_BLOCK_SIZE, SEEK_SET);
	write_msd(0, (uint32_t *)buf.raw, 1);

	memset(buf.raw, 0, SDHC_BLOCK_SIZE);
	
	/* Read the bootsector */
	seek_msd(0, lba / SDHC_BLOCK_SIZE, SEEK_SET);
	read_msd(0, (uint32_t *) buf.raw, 1);
	format16GB_computeSectors(&buf.parse.BPB, lba);

	/* Write the first FAT only */
	memset(buf.raw, 0, SDHC_BLOCK_SIZE);
	buf.raw[0] = 0xF8;
	buf.raw[1] = 0xFF;
	buf.raw[2] = 0xFF;
	buf.raw[3] = 0x0F;
	
	seek_msd(0, params.ReservedSectors, SEEK_SET);
	write_msd(0, (uint32_t *) buf.raw, 1);

	/* Write the root directory */
	buf.raw[0] = 0;
	buf.raw[1] = 0;
	buf.raw[2] = 0;
	buf.raw[3] = 0;

	seek_msd(0, params.FirstDataSector, SEEK_SET);
	write_msd(0, (uint32_t *) buf.raw, 1);

	return 0;
}
