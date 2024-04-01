#include "../libraries/stdint.h"
#include "../libraries/stdio/stdio.h"
#include "../libraries/disk/asmDisk.h"
#include "../libraries/power/power_options.h"
#include "../libraries/fat12/fat12_read.h"

uint16_t cmd_lgt = 0;
char current_directory[11];
char c[100];

void _cdecl cstart_(){
	uint8_t error = 0;

	x86_Disk_Reset(0, error);

	printf("Welcome to BAPOS%n");
	printf("Disk Error: %i", error);
	if (error == 0)
	{
		printf("%nPress any key to continue...%n");
		read_key();
		command_interperter();
	}
	else
	{
		printf("%nPress any key to reboot...%n");
	}
}

void command_interperter(){
	command_beginning:
	printf("%s", current_directory);
	printf("\>");
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
			if(cmd_lgt > 0)
			{
				cmd_lgt = cmd_lgt - 1;
				c[cmd_lgt] = 0;
				putc(c[cmd_lgt]);
			}
		}
		else{
			cmd_lgt++;
		}

		if (cmd_lgt > 98)
		{
			printf("You've reached the command word limit %n");
		}
	}
}

void find_command(){
	char* cmd;
	char* arg;

	arg = strspl(c, " ");
	cmd = c;
	execute_command(cmd, arg);
}

void execute_command(char cmd[], char arg[])
{
	int i = 0;
	// opens directory
	if(strcmp(cmd, "cd")){
		for (i = 0; i < 11; i++)
		{
			current_directory[i] = arg[i];
		}
	}
	// shows contents of the current directory
	else if(strcmp(cmd, "ls")){
		
	}

	// shutdown system
	else if(strcmp(cmd, "shutdown")){
		clear_screen();
		x86_Shutdown();
	}

	// echo back strings
	else if(strcmp(cmd, "echo")){
		printf("%s%n", arg);
	}

	else if(strcmp(cmd, "clear")){
		clear_screen();
	}

	else if(strcmp(cmd, "help")){

		if(*arg == 13){
			printf("echo%nclear%nshutdown%ncd%nls%n");
		}
	}

	else{
		printf("'%s' is not a valid command%n type 'help' to see available commands%n", cmd);
	}
	cmd_lgt = 0;
	for(i = 0; i < 200; i++)
	{
		c[i] = 0;
	}
}

void _cdecl clear_screen_(){
	clear_screen();
}

void _cdecl putc_(char c){
	putc(c);
}
