#include "library/stdint.h"
#include "library/stdio/stdio.h"
#include "disk/asmDisk.h"
#include "power/power_options.h"

void _cdecl cstart_(){
	uint8_t error;
	char c;
	
	clear_screen();

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
			
			if(c == 'Q'){
				x86_Shutdown();
			}
			if(c == 's' || c == 'S'){
				printf("%nLoading program!");
				x86_Disk_Read();
			}
		}
	}
	else
	{
		printf("%nPress any key to reboot...");
	}
}
