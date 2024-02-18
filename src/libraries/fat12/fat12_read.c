#include "fat12_read.h"
#include "../disk/asmDisk.h"
#include "../stdio/stdio.h"

uint16_t fat12_find(char filename[])
{
	// load root directory into memory
	uint16_t sector;
	uint16_t directory_sector;
	unsigned char far* directory;
	unsigned char far* directorycopy;
	uint16_t i;
	uint16_t t;
	directory_sector = SECTORS_PER_FAT * FAT_COUNT + RESERVED_SECTORS;
	x86_Disk_Read(directory_sector, REPOSITORY_SIZE, LOAD_OFFSET_TABLE, LOAD_SEGMENT_TABLE);

	// go through root directory entries and find a matching one
	printf("LOAD_SEGMENT_TABLE: %x %n", LOAD_SEGMENT_TABLE);
	directory = (unsigned char far*)0x40000000;
	printf("directory start: %x %n", directory);
	read_key();
	directorycopy = directory;
	for(i = 0; i < 224; i++)
	{
		printf("directory entry: %x %n", directory);
		read_key();
		for(t = 0; t <= 11; t++)
		{
			printf("filename_char: %c, directory_char: %c %n", filename[t], *directorycopy);
			if(t == 11)
			{
				printf("directory found! %n");
				read_key();
				directory = directory + 26;
				sector = *directory;
				return sector;
			}
			if(filename[t] == *directorycopy)
			{
				printf("Matching char: %c %n", *directorycopy);
				read_key();
				directorycopy++;
			}
			else{
			break;
			}
		}
		printf("Moving to next directory %n");
		directory = directory + 32;
		printf("Next directory: %x %n", directory);
		read_key();
		directorycopy = *directory;
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
