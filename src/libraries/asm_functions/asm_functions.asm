[bits 16]

sections _TEXT class=CODE
global jump
jump:
	jmp [bp + 4]

global jump_program
jump_program:
	mov ax, [bp + 4]
	mov bx, [bp + 6]
	mov es, ax
	mov ss, ax
	mov bp, bx
	mov sp, bp

	jump es:bx
