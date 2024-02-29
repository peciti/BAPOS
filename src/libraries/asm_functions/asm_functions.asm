[bits 16]
KERNSEG	equ 0x6000

section _TEXT class=CODE

global _call_program
_call_program:
	mov ax, [bp + 4] ; segment
	mov bx, [bp + 6] ; offset

	mov ds, ax
	mov es, ax

	mov [JMP_SEGMENT], es
	mov [JMP_OFFSET], bx

	jmp JMP_SEGMENT:JMP_OFFSET

	mov ax, KERNSEG
	mov es, ax
	mov ds, ax

	ret


JMP_SEGMENT: dw 0
JMP_OFFSET: dw 0
