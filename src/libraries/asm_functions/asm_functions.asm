[bits 16]

sections _TEXT class=CODE
global jump
jump:
	jmp [bp + 4]
