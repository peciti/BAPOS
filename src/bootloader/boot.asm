[org 0x7c00]
[bits 16]

stack:
	mov ax, 0
	mov ss, ax
	mov sp, 0x7c00

main:
	mov si, hello_msg
	
	call print
	jmp $

print:
	lodsb
	cmp al, 0
	je .done
	
	mov ah, 0x0e
	int 0x10
	jmp print

.done:
	ret

hello_msg: db 'Hello World!', 0 

times 510-($-$$) db 0
db 0x55, 0xaa
