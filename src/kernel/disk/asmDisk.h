#pragma once
#include "../library/stdint.h"

void _cdecl x86_Disk_Reset(uint8_t drive, uint8_t* error);
void _cdecl x86_Disk_Read();
void _cdecl x86_Jump(uint16_t destination);
