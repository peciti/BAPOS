#include "fat12_read.h"
#include "../disk/asmDisk.h"
#include "../stdio/stdio.h"

uint16_t fat12_find(char* filename)
{
	// load root directory into memory
	uint16_t sector;
	uint16_t directory_sector;
	uint16_t* filenamecopy;
	uint32_t* directory;
	uint32_t* directorycopy;
	uint16_t i;
	uint16_t t;
	directory_sector = SECTORS_PER_FAT * FAT_COUNT + RESERVED_SECTORS;
	x86_Disk_Read(directory_sector, REPOSITORY_SIZE, LOAD_OFFSET_TABLE, LOAD_SEGMENT_TABLE);

	// go through root directory entries and find a matching one
	filenamecopy = *filename;
	directory = LOAD_SEGMENT_TABLE * 16 + LOAD_OFFSET_TABLE;
	directorycopy = *directory;
	for(i = 0; i < 224; i++)
	{
		printf("directory entry: %x %n", directory);
		for(t = 0; t < 11; t++)
		{
			if(t == 11)
			{
				directory = directory + 26;
				sector = &directory;
				return sector;
			}
			if(&filenamecopy != &directorycopy)
			{
				continue;
			}
			filenamecopy++;
			directorycopy++;
		}
		directory = directory + 32;
		directorycopy = *directory;
		filenamecopy = *filename;
	}
	// sector value if file was not found
	sector = 64000;
	return sector;
}

void fat12_read(uint16_t sector, uint16_t load_segment, uint16_t load_offset)
{
	if(sector == 64000)
	{
		printf("File not found!%n");
	}
	else
	{
		// load FAT into memory
		uint16_t FAT_sector;
		FAT_sector = RESERVED_SECTORS;
		x86_Disk_Read(FAT_sector, SECTORS_PER_FAT, LOAD_OFFSET_TABLE, LOAD_SEGMENT_TABLE);
		printf("Loading File%n");
		while(1)
		{
			sector = sector + 31;
			if(sector == 0x0ff8)
			{
				break;
			}
			x86_Disk_Read(sector, 1, load_offset, load_segment);
			load_offset = load_offset + 512; // bytes per sector
		}
	}
}

void run_program(char* filename, uint16_t load_segment, uint16_t load_offset)
{
uint16_t load_segment_copy;
uint16_t load_offset_copy;
load_segment_copy = load_segment;
load_offset_copy = load_offset;
fat12_read(fat12_find(filename), load_segment, load_offset);

}
