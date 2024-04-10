#include "../libraries/stdint.h"
#include "../libraries/stdio/stdio.h"
#include "../libraries/disk/asmDisk.h"
#include "../libraries/power/power_options.h"
#include "../libraries/fat12/fat12.h"

uint16_t cmd_len = 0;
uint16_t page_num = 0;
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
	printf("%s/>", current_directory);
	while(1){
		c[cmd_len] = read_key();
		putc(c[cmd_len]);

		// Enter
		if(c[cmd_len] == 13){
			printf("%n");
			find_command();
			goto command_beginning;
		}
		// Backspace
		if(c[cmd_len] == 8){
			if(cmd_len > 0)
			{
				cmd_len = cmd_len - 1;
				c[cmd_len] = 0;
				putc(c[cmd_len]);
			}
		}
		else{
			cmd_len++;
		}

		if (cmd_len > 98)
		{
			printf("%nYou've reached the command word limit %n");
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
		display_directory(current_directory);		
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
	else if(strcmp(cmd, "run")){
		run_program(arg, 0x1000, 0x0);
	}

	else if(strcmp(cmd, "help")){

		if(*arg == 13){
			printf("echo%nclear%nshutdown%ncd%nls%nrun%n");
		}
	}

	else{
		printf("'%s' is not a valid command%n type 'help' to see available commands%n", cmd);
	}
	cmd_len = 0;
	for(i = 0; i < 100; i++)
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
