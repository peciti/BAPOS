#pragma once
#include "../stdint.h"

uint16_t fat12_find(char* filename); // searches root directory and returns the first fat cluster
void fat12_read(uint16_t cluster, uint16_t load_segment, uint16_t load_offset); // loads from disk into memory
void run_program(char* filename, uint16_t load_segment, uint16_t load_offset); // loads program into memory and jumps to it

#define LOAD_SEGMENT_TABLE 0x4000 // where root directory and FAT will be loaded
#define LOAD_OFFSET_TABLE 0x0

#define SECTORS_PER_FAT 9
#define BYTES_PER_SECTOR 512
#define RESERVED_SECTORS 1
#define FAT_COUNT 2
#define REPOSITORY_SIZE 14
