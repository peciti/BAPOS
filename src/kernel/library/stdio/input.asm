bits 16

section _TEXT class=CODE

global _x86_Read_Keypress
_x86_Read_Keypress:

	push bp
	mov bp, sp

	mov ah, 0x0
	int 0x16

	mov [bp + 6], ax

	mov sp, bp

	pop bp
	ret

global _x86_Jump
_x86_Jump:

	jmp [bp]
	ret
