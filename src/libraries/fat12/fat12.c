#include "fat12.h"
#include "../disk/asmDisk.h"
#include "../stdio/stdio.h"

__segment segment_table = LOAD_SEGMENT_TABLE;
char __based(segment_table)* table_ptr;

void disk_read(uint16_t sector, uint16_t size, uint16_t offset, uint16_t segment){
	x86_Disk_Read(sector, size, offset, segment);
}

void disk_write(uint16_t sector, uint16_t size, uint16_t offset, uint16_t segment){
	x86_Disk_Read(sector, size, offset, segment);
}

void load_directory()
{
	uint16_t directory_sector;
	directory_sector = SECTORS_PER_FAT * FAT_COUNT + RESERVED_SECTORS;
	disk_read(directory_sector, REPOSITORY_SIZE, LOAD_OFFSET_TABLE, LOAD_SEGMENT_TABLE);
}

void load_FAT()
{
	uint16_t FAT_sector;
	FAT_sector = RESERVED_SECTORS;
	disk_read(FAT_sector, SECTORS_PER_FAT, LOAD_OFFSET_TABLE, LOAD_SEGMENT_TABLE);
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
			// printf("%c and %c", *directorycopy, filename[t]);
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
		//load_FAT();
		
		//while(1)
		//{
			sector = sector + 31;
			printf("sector: %i%n", sector);
			printf("Loading at: %x:%x%n", load_segment, load_offset);

			/*if(sector == 0x0ff8)
			{
				break;
			}*/
			
			// x86_Disk_Read(sector, 1, load_offset, load_segment);
			disk_read(sector, 1, load_offset, load_segment);
			// load_offset = load_offset + 512; // bytes per sector
			// break;
		//}
		
		return 1;
	}
	return 0;
}

void convert_filename(char* filename)
{
	uint16_t i;
	uint16_t filename_len;
	uint16_t buffer_len;
	char* filenamecopy;
	char* format;
	char* formatcopy;
	char formatarraycopy[3];
	format = strspl(filename, ".");
	
	str_lower_to_upper(filename);
	str_lower_to_upper(format);

	filenamecopy = filename;
	formatcopy = format;

	for(i = 0; i < 3; i++)
	{
		formatarraycopy[i] = *formatcopy;
		formatcopy++;
	}

	filename_len = strlen(filename);
	for(i = 0; i < filename_len; i++)
	{
		filenamecopy++;
	}

	buffer_len = 8 - filename_len;
	for(i = 0; i < buffer_len; i++)
	{
		*filenamecopy = 0x20;
		filenamecopy++;
	}

	for(i = 0; i < strlen(format); i++)
	{
		*filenamecopy = formatarraycopy[i];
		filenamecopy++;
	}
}

void create_file(char filename[])
{
	uint16_t i;
	uint16_t t;
	uint16_t r;
	__segment loadseg = LOAD_SEGMENT_TABLE;
	char __based(loadseg)* directory = 0;
	char __based(loadseg)* directorycopy;
	uint16_t __based(loadseg)* FAT_entry = 0x3;
	load_directory();
	directorycopy = directory;
	convert_filename(filename);
		for(i = 0; i < 224; i++)
		{
			if(*directorycopy == 0)
			{
				for(t = 0; t < 11; t++){
					*directorycopy = filename[t];
					directorycopy++;
				}
				directorycopy = directorycopy + 26;

				load_FAT();
				while(*FAT_entry != 0x0)
				{
					r = *FAT_entry % 2;
					if(r != 0)
					{
						*FAT_entry >> 4;
					}
					FAT_entry++;
				}
				r = FAT_entry;
				if(*FAT_entry == 0xff8)
				{
					printf("Not enough space on disk");
				}
				
				load_directory();
				*directorycopy = r;
				directorycopy = directory;
				for(t = 0; t < 11; t++){
					*directorycopy = filename[t];
					directorycopy++;
				}
				
				x86_Disk_Write(SECTORS_PER_FAT * FAT_COUNT + RESERVED_SECTORS, REPOSITORY_SIZE, LOAD_OFFSET_TABLE, LOAD_SEGMENT_TABLE);
				printf("File created%n");
				break;
			}
			
			directory = directory + 32;
			directorycopy = directory;
		}	
}

void write_file(char filename[], char* data)
{
	__segment writeseg = segment_table;
	char __based(writeseg)* write_ptr = table_ptr;
	uint16_t first_sector;
	
	convert_filename(filename);
	printf("%s%n", filename);
	printf("%s%n", data);
	
	first_sector = fat12_find(filename);
	fat12_read(first_sector, writeseg, 0x0);
	while(*data)
	{
		*write_ptr = *data;
		write_ptr++;
		data++;
	}
	
	x86_Disk_Write(first_sector + 31, 1, 0x0, writeseg);
}

void delete_file(char filename[])
{
	uint16_t first_sector;
	uint8_t i;
	__segment deleteseg = segment_table;
	char __based(deleteseg)* delete_ptr = table_ptr;
	
	convert_filename(filename);
	printf("%s%n", filename);
	load_directory();
	printf("%s", delete_ptr);
	read_key();
	
	while(!strcmp(delete_ptr, filename))
	{
		printf("%s", delete_ptr);
		delete_ptr = delete_ptr + 32;
	}

	if(strcmp(delete_ptr, filename))
	{

		printf("%s and %s", delete_ptr, filename);
		for(i = 0; i < 11; i++)
		{
			*delete_ptr = 0;
			delete_ptr++;
		}
		
		x86_Disk_Write(SECTORS_PER_FAT * FAT_COUNT + RESERVED_SECTORS, REPOSITORY_SIZE, LOAD_OFFSET_TABLE, LOAD_SEGMENT_TABLE);
		printf("File deleted");
	
	}
}

void dump_file(char filename[])
{
	uint16_t i;
	uint16_t first_sector;
	__segment dumpseg = segment_table;
	char __based(dumpseg)* dump_ptr = table_ptr;
	
	convert_filename(filename);
	printf("%s%n", filename);
	first_sector = fat12_find(filename);
	
	fat12_read(first_sector, dumpseg, table_ptr);
	printf("%i%n", first_sector);
	// x86_Disk_Read(first_sector, 1, LOAD_OFFSET_TABLE, LOAD_SEGMENT_TABLE);
	
	// fat12_read(first_sector, dumpseg, 0x0);
	/*while(*dump_ptr)
	{
		printf("%c", *dump_ptr);
		dump_ptr++;
	}*/
	for(i = 0; i < 512; i++)
	{
		printf("%c", *dump_ptr);
		dump_ptr++;
	}
}

void run_program(char* filename, uint16_t load_segment, uint16_t load_offset)
{
	bool success;
	convert_filename(filename);
	success = fat12_read(fat12_find(filename), load_segment, load_offset);
	if(success)
	{
		// doesn't work yet (doesn't jump to the program or maybe program isn't loaded?)
		__asm{	
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

			}
	}
}
