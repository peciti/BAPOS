#pragma once
#include "../stdint.h"

void putc(char c);
void puts(const char* str);
void _cdecl printf(const char* fmt, ...);
void puts_f(const char far* s);
uint16_t strlen(const char* str);
char* strspl(const char* str, const char* del);
bool strcmp(const char str1[], const char str2[]);
int * printf_number(int*, int, bool, int);
void clear_screen();
char read_key();

#define PRINTF_STATE_START 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_SHORT 2
#define PRINTF_STATE_LONG 3
#define PRINTF_STATE_SPEC 4

#define PRINTF_LENGTH_START 0
#define PRINTF_LENGTH_SHORT_SHORT 1
#define PRINTF_LENGTH_SHORT 2
#define PRINTF_LENGTH_LONG 3
#define PRINTF_LENGTH_LONG_LONG 4
