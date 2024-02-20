[bits 16]
[org 0x0]

jmp short start

start:
	mov si, hello_world

print:
	lodsb
	cmp al, 0
	je .done
	mov ah, 0x0e
	int 0x10
	jmp print

.done:
	jmp halt

halt:
	jmp halt
hello_world: db 'Hello World!', 0

