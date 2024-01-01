#include "library/stdint.h"
#include "library/stdio/stdio.h"
#include "disk/asmDisk.h"

void _cdecl cstart_(){
	uint8_t error;
	
	clear_screen();

	x86_Disk_Reset(0, error);
	
	printf("Kernel Loaded! Welcome to BAPOS");
	printf("%nDisk Error: %i", error);
	if (error == 0)
	{
		printf("%nPress any key to continue...");
	}
	else
	{
		printf("%nPress any key to reboot...");
	}
}
