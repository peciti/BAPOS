#pragma once
#include "../stdint.h"

void disk_read(uint16_t sector, uint16_t size, uint16_t offset, uint16_t segment);
void disk_write(uint16_t sector, uint16_t size, uint16_t offset, uint16_t segment);
void display_directory(char current_directory[]); // prints out to the screen contents of given directory
void load_directory(char current_directory[]); // loads root directory into memory
void load_FAT();
uint16_t fat12_find(char* filename); // searches root directory and returns the first fat cluster
bool fat12_read(uint16_t cluster, uint16_t load_segment, uint16_t load_offset); // loads from disk into memory
void run_program(char filename[], uint16_t load_segment, uint16_t load_offset); // loads program into memory and jumps to it
void convert_filename(char* filename);
void create_file(char* filename);
void dump_file(char filename[]);
void write_file(char filename[], char* data);
void delete_file(char filename[]);

#define LOAD_SEGMENT_TABLE 0x4000 // where root directory and FAT will be loaded
#define LOAD_OFFSET_TABLE 0x0

#define SECTORS_PER_FAT 9
#define BYTES_PER_SECTOR 512
#define RESERVED_SECTORS 1
#define FAT_COUNT 2
#define REPOSITORY_SIZE 14
#define KERNSEG 0x6000
