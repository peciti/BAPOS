#include "library/stdint.h"
#include "library/stdio/stdio.h"
#include "disk/asmDisk.h"
#include "power/power_options.h"

void _cdecl cstart_(){
	uint8_t error;
	char c;

	x86_Disk_Reset(0, error);
	
	printf("Kernel Loaded! Welcome to BAPOS");
	printf("%nDisk Error: %i", error);
	if (error == 0)
	{
		printf("%nPress any key to continue...");
		c = read_key();
		while(1){
			c = read_key();
			putc(c);
			
			if(c == 17){
				x86_Shutdown();
			}
			if(c == 13){
				clear_screen();
			}
			if(c == 'A'){
				_asm{
						xor ax, ax
						mov es, ax
						jmp 0x7c00
					}
			}
		}
	}
	else
	{
		printf("%nPress any key to reboot...");
	}
}

void _cdecl clear_screen_(){
	clear_screen();
}

void _cdecl putc_(char c){
	putc(c);
}
