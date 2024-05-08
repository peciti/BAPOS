#include "fat12.h"
#include "../disk/asmDisk.h"
#include "../stdio/stdio.h"
#include "../asm_functions/asm_functions.h"

void load_directory()
{
	uint16_t directory_sector;
	directory_sector = SECTORS_PER_FAT * FAT_COUNT + RESERVED_SECTORS;
	x86_Disk_Read(directory_sector, REPOSITORY_SIZE, LOAD_OFFSET_TABLE, LOAD_SEGMENT_TABLE);
}

void display_directory(char current_directory[])
{
	uint16_t i;
	uint16_t t;
	__segment directoryseg = LOAD_SEGMENT_TABLE;
	char __based(directoryseg)* directory = 0;
	char __based(directoryseg)* directorycopy;
	load_directory();
	directorycopy = directory;
	
	if(current_directory[0] == 0)
	{
		for(i = 0; i < 224; i++){
			if(*directorycopy == 0)
			{
				directory = directory + 32;
				directorycopy = directory;
				continue;
			}
		
			for(t = 0; t <= 11; t++){
				if(*directorycopy != 0)
				{
					printf("%c", *directorycopy);	
				}
				directorycopy++;
			}
			
			printf("%n");
			directory = directory + 32;
			directorycopy = directory;
		}
	}
}

uint16_t fat12_find(char filename[])
{
	// load root directory into memory
	uint16_t sector;
	__segment directoryseg = LOAD_SEGMENT_TABLE;
	char __based(directoryseg)* directory;
	char __based(directoryseg)* directorycopy;
	uint16_t i;
	uint16_t t;
	load_directory();

	// go through root directory entries and find a matching one
	directory = LOAD_OFFSET_TABLE;
	directorycopy = directory;
	for(i = 0; i < 224; i++)
	{
		for(t = 0; t <= 11; t++)
		{
			printf("%c and %c", *directorycopy, filename[t]);
			if(t == 11)
			{
				directory = directory + 26;
				sector = *directory;
				return sector;
			}
			if(filename[t] == *directorycopy)
			{
				directorycopy++;
			}
			else{
			break;
			}
		}
		directory = directory + 32;
		directorycopy = directory;
	}
	// sector value if file was not found
	sector = 64000;
	return sector;
}

bool fat12_read(uint16_t sector, uint16_t load_segment, uint16_t load_offset)
{
	if(sector == 64000)
	{
		printf("File not found!%n");
		return 0;
	}
	else
	{
		// load FAT into memory
		uint16_t FAT_sector;
		__segment FAT_segment = LOAD_SEGMENT_TABLE;
		uint16_t __based(FAT_segment)* FATptr;
		FAT_sector = RESERVED_SECTORS;
		x86_Disk_Read(FAT_sector, SECTORS_PER_FAT, LOAD_OFFSET_TABLE, LOAD_SEGMENT_TABLE);
		printf("Loading File%n");
		while(1)
		{
			sector = sector + 31;
			printf("sector: %x %n", sector);
			printf("Loading at: %x:%x %n", load_segment, load_offset);
			read_key();

			/*if(sector == 0x0ff8)
			{
				break;
			}*/
			x86_Disk_Read(sector, 1, load_offset, load_segment);
			// load_offset = load_offset + 512; // bytes per sector
			break;
		}
		printf("File Loaded press any key to continue...%n");
		read_key();
		return 1;
	}
	return 0;
}

void run_program(char* filename, uint16_t load_segment, uint16_t load_offset)
{
	bool success;
	success = fat12_read(fat12_find(filename), load_segment, load_offset);
	if(success)
	{
		// doesn't work yet (doesn't jump to the program or maybe program isn't loaded?)
		__asm{
			push ax
			push bx
			push cx
			push dx
	
			mov ax, load_segment
	
			mov ds, ax
			mov es, ax
	
			mov bx, load_offset
	
			call es:bx
	
			mov ax, KERNSEG
			mov es, ax
			mov ds, ax	
	
			cli
			pop ax
			mov sp, ax
			mov ax, KERNSEG
			mov ss, ax
			sti
	
			pop dx
			pop cx
			pop bx
			pop ax
			}
	}
}
