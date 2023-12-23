[bits 16]

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
