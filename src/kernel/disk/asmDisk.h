#pragma once
#include "../library/stdint.h"

void _cdecl x86_Disk_Reset(uint8_t drive, uint8_t* error);
void _cdecl x86_Disk_Read(uint8_t drive, uint8_t* error, uint8_t lba_address, uint8_t count);
