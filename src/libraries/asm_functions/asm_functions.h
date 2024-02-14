#pragma once
#include "../stdint.h"

void _cdecl jump(uint16_t offset);
void _cdecl jump_program(uint16_t segment, uint16_t offset);
