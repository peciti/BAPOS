#include "fat12_read.h"
#include "../disk/asmDisk.h"

uint16_t fat12_find(char* filename)
{
	uint16_t sector;
	uint16_t directory_sector;
	directory_sector = SECTORS_PER_FAT * FAT_COUNT + RESERVED_SECTORS;
	x86_Disk_Read(directory_sector, REPOSITORY_SIZE, LOAD_OFFSET_TABLE, LOAD_SEGMENT_TABLE);

	return sector
}

void fat12_read(uint16_t sector, uint16_t load_segment, uint16_t load_offset)
{

}
