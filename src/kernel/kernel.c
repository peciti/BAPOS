#include "../libraries/stdint.h"
#include "../libraries/stdio/stdio.h"
#include "../libraries/disk/asmDisk.h"
#include "../libraries/power/power_options.h"

void _cdecl cstart_(){

}

void _cdecl clear_screen_(){
	clear_screen();
}

void _cdecl putc_(char c){
	putc(c);
}
