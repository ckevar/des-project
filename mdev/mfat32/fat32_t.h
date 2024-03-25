#ifndef _FAT32_T_H_
#define _FAT32_T_H_

#define FAT_DIR_NAME_SIZE 	11

struct PartionRecords {
	// fat32's data 
	uint8_t 	boot; 		// bootable partion
	uint8_t 	start_chsH; // start of the partion in CHS
	uint16_t 	start_chsL;
	uint8_t 	fs_type;	// file system type, fatX or exFAT
	uint8_t 	end_chsH;	// end of the partion in CHS
	uint16_t 	end_chsL;
	uint32_t 	lba;		// relative offset, where FAT starts
	uint32_t 	size; 		// number of sectors
} __attribute__((packed));


struct BootSector {
	// Classical FAT
	uint8_t 	BootJumpInstruction[3]; 	
	uint8_t 	OemIdentifier[8]; 			
	uint16_t 	BytesPerSector;			
	uint8_t 	SectorsPerCluster;	
	uint16_t 	ReservedSectors; 	
	uint8_t 	NumFATs; 			
	uint16_t 	DirEntryCount; 		
	uint16_t 	TotalSectors; 		
	uint8_t 	MediaDescriptorType; 
	uint16_t 	SectorsPerFat; 		
	uint16_t 	SectorsPerTrack;	
	uint16_t 	Heads; 			
	uint32_t 	HiddenSectors;	
	uint32_t 	LargeSectorCount; 

	// FAT32
	uint32_t 	FATSz32; 				// Occupied Sectos by ONE FAT
	uint16_t 	ExtFlags; 				// Mirroring related 	
	uint16_t 	FSVersion;				// FAT32 version 	
	uint32_t 	RootClus; 				// Cluster Number of the first cluster 
	uint16_t 	FSInfo; 				
	uint16_t 	BkBootSec; 
	uint8_t		Reserved[12];
	uint8_t 	DrvNum;
	uint8_t 	Reseverd1;
	uint8_t 	BootSig;
	uint32_t 	VolID;
	uint8_t 	VolumeLabel[11];    
	uint8_t 	SystemId[8]; 			

} __attribute__((packed));

struct DIR {
	uint8_t Name[FAT_DIR_NAME_SIZE];
	uint8_t Attributes;
	uint8_t _Reserved;
	uint8_t CreatedTimeTenths;
	uint16_t CreatedTime;
	uint16_t CreatedDate;
	uint16_t AccessedDate;
	uint16_t FirstClusterHigh;
	uint16_t ModifiedTime;
	uint16_t ModifiedDate;
	uint16_t FirstClusterLow;
	uint32_t Size;
} __attribute__((packed));
     


/** 512-byte wrappers **/
struct MBRBlock {
	uint8_t stuff0[416];
	uint8_t stuff1[30];
	struct PartionRecords MBR[4];
	uint16_t _55AA; 			// due to endianess, this prints 0xAA55
} __attribute__((packed));

struct BootSectorBlock {
	struct BootSector BPB;
	uint8_t stuff[420];
	uint16_t _55AA; 		// due to endianess, this 0xAA55
} __attribute__((packed));

#endif



