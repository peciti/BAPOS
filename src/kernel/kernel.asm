[org 0x7e00]
[bits 16]

main:
	mov si, hello_msg
	call print
	
	jmp halt
	
print:
	lodsb
	cmp al, 0
	je .done
	
	mov ah, 0x0e
	int 0x10
	jmp print
	
.done:
	ret

halt:
	hlt

hello_msg: db 'Hello World!', 0
