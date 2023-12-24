bits 16

section _TEXT class=CODE

global _x86_div64_32
_x86_div64_32:
	push bp
	mov bp, sp

	push bx

	mov eax, [bp+8]	; upper 32 bits of dividend
	mov ecx, [bp+12] ; divisor
	xor edx,edx
	div ecx

	mov bx, [bp+16] ; upper 32 bits of quotient
	mov [bx+4], eax

	mov eax, [bp+4] ; lower 32 bits of dividend
	div ecx

	mov [bx], eax
	mov bx, [bp+18]
	mov [bx], edx

	pop bx
	mov sp, bp
	pop bp
	ret
	
global _x86_Write_Character
_x86_Write_Character:
	push bp
	mov bp, sp

	push bx
	mov ah, 0x0e
	mov al, [bp + 4]
	mov bh, [bp + 6]
	
	int 0x10

	pop bx
	mov sp, bp

	pop bp
	
	ret
