%define endl 0x0d, 0x0a
[org 0x00]
[bits 16]

global entry

entry:
	jmp 0x00:start

start:
	cli
	mov ax, ds
	mov ss, ax
	mov sp, 0
	mov bp, sp
	sti

	xor ax, ax
	mov al, 0x00
	int 0x10
	xor bx, bx

	jmp 0x00:main

main:
	mov si, hello_msg
	call print

	jmp end

print:
	lodsb
	cmp al, 0
	je .done
	mov ah, 0x0e
	int 0x10

	jmp print

.done:
	ret

end:
	push 1
	ret

hello_msg: db 'Hello World!', endl, 0

