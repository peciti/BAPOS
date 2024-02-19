[bits 16]

sections _TEXT class=CODE
global jump
jump:
	jmp [bp + 4]

global jump_program
jump_program:

