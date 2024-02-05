#pragma once
#include "../stdint.h"

void fat12_find(char* filename);
void fat12_read(uint16_t cluster);

#define LOAD_SEGMENT 0x2000
#define LOAD_OFFSET 0x0
