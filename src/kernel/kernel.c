#include "../libraries/stdint.h"
#include "../libraries/stdio/stdio.h"
#include "../libraries/disk/asmDisk.h"
#include "../libraries/power/power_options.h"
#include "../libraries/fat12/fat12_read.h"

uint16_t cmd_lgt;
char current_directory[11];
char c[200];

void _cdecl cstart_(){
	uint8_t error;

	x86_Disk_Reset(0, error);

	printf("Kernel Loaded! Welcome to BAPOS");
	printf("%nDisk Error: %i", error);
	if (error == 0)
	{
		printf("%nPress any key to continue...");
		read_key();
		command_interperter();
	}
	else
	{
		printf("%nPress any key to reboot...");
	}
}

void command_interperter(){
	command_beginning:
	printf("\%s>", current_directory);
	while(1){
		c[cmd_lgt] = read_key();
		putc(c[cmd_lgt]);

		// Enter
		if(c[cmd_lgt] == 13){
			printf("%n");
			find_command();
			goto command_beginning;
		}
		// Backspace
		if(c[cmd_lgt] == 8){
			cmd_lgt = cmd_lgt - 1;
			c[cmd_lgt] = 0;
			putc(c[cmd_lgt]);
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

	arg = strspl(c, ' ');
	cmd = c[0];
	execute_command(cmd, arg);
}

void execute_command(const char cmd[], const char arg[])
{
	// will remove this later
	printf("%s %s %n", cmd, arg);

	//show contents of the current directory
	if(strcmp(cmd, "ls")){
	}

	// shutdown system
	else if(strcmp(cmd, "qu")){
		x86_Shutdown();
	}
	// echo back strings
	else if(strcmp(cmd, "ec")){
		printf("%s%n", arg);
	}
	else{
		printf("'%s' is not a valid command%n", cmd);
	}
}

void _cdecl clear_screen_(){
	clear_screen();
}

void _cdecl putc_(char c){
	putc(c);
}
