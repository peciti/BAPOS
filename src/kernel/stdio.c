#include "stdio.h"
#include "print.h"

void putc(char c){
	x86_Write_Character(c,0);
}

void puts(const char* s){
	while(*s)
	{
		putc(*s);
		s++;
	}
}
