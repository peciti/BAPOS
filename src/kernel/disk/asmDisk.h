#pragma once
#include "../library/stdint.h"

void _cdecl x86_Disk_Reset(uint8_t drive, uint8_t* error);
void _cdecl x86_Disk_Read(uint16_t drive, uint16_t lba_address, uint16_t count, uint16_t destination);
