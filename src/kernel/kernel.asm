[bits 16]

section _ENTRY CLASS=CODE

extern _cstart_
extern _interrupt_initialize
extern _clear_screen_
extern _putc_

global entry

jmp entry ; 0x0
jmp system_call ; 0x2
jmp screen_call ; 0x4

entry:
	cli
	mov ax, ds
	mov ss, ax
	mov sp, 0
	mov bp, sp
	sti

	call _interrupt_initialize
	call _cstart_

	cli
	hlt

halt:
	jmp halt

system_call:
	cmp ax, 0 ; clear screen system call
	je _clear_screen_
	ret	

screen_call:
	call _putc_ ; find a way to pass argument to the method
	ret
