/*********************************************************************
 * The skeleton of this file was generated automatically for the mdev    
 * framework that supports the module:
 *		fat32
 * 
 * Documentation:
 * 1. 	Microsoft Extensible Firmware Initiative FAT32 File System 
 * 		specification:
 *  	https://www.cs.fsu.edu/~cop4610t/assignments/project3/spec/fatspec.pdf
 * 
 * 2.	Understanding FAT32 Filesystems:
 * 		https://www.pjrc.com/tech/8051/ide/fat32.html
 *
 * 3. 	Implementing FAT on CompactFlash cards, SD/MMC cards and USB sticks:
 * 		https://www.compuphase.com/mbr_fat.htm
 *
 * 4. 	Simple FAT and SD Tutorial Part 1-2:
 * 		https://codeandlife.com/2012/04/02/simple-fat-and-sd-tutorial-part-1/
 *
 * 5. 	Nanobyte_os:
 * 		https://github.com/nanobyte-dev/nanobyte_os/blob/videos/part3/tools/fat/fat.c#L82
 * 		
 * File generated on 2024-02-21 18:33:05.419548
 * With mdev generator version 0.1.4 from 2024-02-21 08:41:07.842164
 ********************************************************************/

/*
 * THIS IS A UTILITY:
 * It's used only to check where can we store data without damaging the 
 * format.
 */

#include "mdev.h"
#include "mfat32/fat32.h"
#include "mfat32/fat32_t.h"
// #include "mfat32/format16GB.h"
#include "drivers/sd.h"

#include <string.h>

struct fat32 {
	uint32_t num_sectors;
	uint32_t ReservedSectors;
	uint32_t FirstDataSector;
	uint8_t SectorsPerCluster;
};

static struct fat32 fat32s;

/* read file */
uint32_t read_mfat32(uint32_t fa, uint8_t *buf, uint32_t size) {
	if (fa == 0) 
		return 0;
}

/* open file */
// It only supports:
//
// 	1. Short Names
// 	2. Root Directory
void to_DIR_name(char *dest, const char *src) {
	uint8_t i, j;

	i = j = 0;

	while(i < FAT_DIR_NAME_SIZE) {
		
		if (src[j] == '.') {
			while(i < 8) {
				dest[i] = ' ';
				i++;
			}
			j++;
		}

		else if (src[j] < '/') {
			dest[0] = 0;
			return;
		}
		
		if (src[j] == 0)
			return;

		dest[i] = src[j];
		j++;
		i++;
	}
}	

int8_t memdir(struct DIR *dir, char *filename, int8_t entry) {
	
	do {
		entry--;

		if (memcmp(filename, dir[entry].Name, FAT_DIR_NAME_SIZE) == 0)
			return entry;

	} while(entry);

	return -1;
}

int8_t touch_dir(struct DIR *dir, const char *filename) {
	uint8_t file_entry;
	
	file_entry = memdir(dir, "\0\0\0\0\0\0\0\0\0\0\0", 16);
	if (file_entry < 0)
		return -1;

	memset(dir + file_entry, 0, sizeof(struct DIR));
	memcpy(dir[file_entry].Name, filename, FAT_DIR_NAME_SIZE);
}

uint32_t open_mfat32(const char *min) {
	union {
		struct DIR parse[16];
		uint8_t raw[SDHC_BLOCK_SIZE];
	} dir;
	char filename[FAT_DIR_NAME_SIZE];
	uint8_t file_entry;

	if (min[0] < 'A') 
		return 0;
	
	to_DIR_name(filename, min);
	if(filename[0] == 0) 
		return 0;

	seek_msd(0, fat32s.FirstDataSector, SEEK_SET);
	read_msd(0, (uint32_t *)dir.raw, 1);
	
	file_entry = memdir(dir.parse, filename, 16);

	if (file_entry < 0) {
		return -1;
	}

	return dir.parse[file_entry].FirstClusterHigh << 16 | \
		   dir.parse[file_entry].FirstClusterLow;
}

/* Module Init */

uint32_t mf32_readMBR (void) {
	union {
		struct MBRBlock parse;
		uint8_t raw[SDHC_BLOCK_SIZE];
	} buf;

	seek_msd(0, 0, SEEK_SET);
	read_msd(0, (uint32_t *) buf.raw, 1);

	// Only the first partion will be used	
	if (buf.parse.MBR[0].fs_type == 0x0B || \
		buf.parse.MBR[0].fs_type == 0x0C) {
		fat32s.num_sectors = buf.parse.MBR[0].size;
		return buf.parse.MBR[0].lba;
	}
	return 0;
}

void mf32_computeSectors(struct BootSector *BPB, uint32_t offset_bytes) {
	fat32s.ReservedSectors = offset_bytes/SDHC_BLOCK_SIZE + BPB->ReservedSectors;
	fat32s.FirstDataSector = fat32s.ReservedSectors + (BPB->NumFATs * BPB->FATSz32);
	fat32s.SectorsPerCluster = BPB->SectorsPerCluster;
}

uint32_t mf32_readBootSector(uint32_t lba) {
	union {
		struct BootSectorBlock parse;
		uint8_t raw[SDHC_BLOCK_SIZE];
	} buf;

	seek_msd(0, lba / SDHC_BLOCK_SIZE, SEEK_SET);
	read_msd(0, (uint32_t *) buf.raw, 1);
	
	mf32_computeSectors(&buf.parse.BPB, lba);

	/* This could mean, the SD needs format */
	if (fat32s.SectorsPerCluster == 0) 
		return 1;

	return 0;
}

uint8_t mf32_readFat(void) {
	uint32_t raw[SDHC_BLOCK_SIZE / 4];
	seek_msd(0, fat32s.ReservedSectors,  SEEK_SET);
	read_msd(0, raw, 1);
}

int8_t ModuleInit_mfat32() {
	uint32_t lba;
	ModuleInit_msd();

	lba = mf32_readMBR();
	if (lba == 0) return 1;

	// mf32_formatF32_SD(lba);
	
	if (mf32_readBootSector(lba)) return 1;
	mf32_readFat();
	return 0;
}
