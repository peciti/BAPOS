#pragma once
#include "../stdint.h"

void _cdecl x86_Disk_Reset(uint8_t drive, uint8_t* error);
void _cdecl x86_Disk_Read(uint16_t sector, uint16_t count, uint16_t offset, uint16_t segment);
