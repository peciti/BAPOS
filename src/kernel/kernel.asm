[bits 16]

section _ENTRY CLASS=CODE

extern _cstart_
extern _clear_screen_
extern _putc_

global entry

jmp entry ; 0x0
jmp _clear_screen_ ; 0x2
jmp _putc_ ; 0x4

entry:
	cli
	mov ax, ds
	mov ss, ax
	mov sp, 0
	mov bp, sp
	sti

	call _clear_screen_
	call _cstart_

	cli
	hlt
	
halt:
	jmp halt
