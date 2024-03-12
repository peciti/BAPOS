#include "../libraries/stdint.h"
#include "../libraries/stdio/stdio.h"
#include "../libraries/disk/asmDisk.h"
#include "../libraries/power/power_options.h"
#include "../libraries/fat12/fat12_read.h"

void _cdecl cstart_(){
	uint8_t error;
	uint16_t cmd_lgt;
	char current_directory[11];
	char c[200];

	x86_Disk_Reset(0, error);

	printf("Kernel Loaded! Welcome to BAPOS");
	printf("%nDisk Error: %i", error);
	if (error == 0)
	{
		printf("%nPress any key to continue...");
		c = read_key();
		command_interperter();
	}
	else
	{
		printf("%nPress any key to reboot...");
	}
}

void command_interperter(){
	command_beginning:
	printf("\%s>", current_directory)
	while(1){
		c[cmd_lgt] = read_key();
		putc(c[cmd_lgt]);

		if(c[cmd_lgt] == 13){
			printf("%n");
			find_command();
			goto command_beginning;
		}
		cmd_lgt++;
		if (cmd_lgt > 198)
		{
			printf("You've reached the command word limit %n");
		}
	}
}

void find_command(){
	char* cmd;
	char* arg;
}

void execute_command(const char cmd[], const char arg[])
{
	switch(cmd){
	case "ls":
	fat12_directory_list();
	break;

	case "ec":
	break;

	default:
	printf("'%s' is not a valid command!%n", cmd);
	break;
	}
}

void _cdecl clear_screen_(){
	clear_screen();
}

void _cdecl putc_(char c){
	putc(c);
}
