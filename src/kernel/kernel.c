#include "../libraries/stdint.h"
#include "../libraries/stdio/stdio.h"
#include "../libraries/disk/asmDisk.h"
#include "../libraries/power/power_options.h"
#include "../libraries/fat12/fat12_read.h"

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
			if(c == 'H')
			{
				run_program("HELLO   BIN", 0x4000, 0x0);
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
