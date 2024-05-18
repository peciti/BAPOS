#pragma once
#include "../stdint.h"

void _cdecl x86_Disk_Reset(uint8_t drive, uint8_t* error);
void _cdecl x86_Disk_Read(uint16_t sector, uint16_t count, uint16_t offset, uint16_t segment);
void _cdecl x86_Disk_Write(uint16_t sector, uint16_t count, uint16_t offset, uint16_t segment); // sector to write, how many, offset and segment from where to get info
