[bits 16]

.entry:
	jmp start

start:
	cli
	mov ax, ds
	mov ss, ax
	mov sp, 0
	sti

	xor ax, ax
	mov al, 0x03
	int 0x10
	xor bx, bx

	jmp main
	ret

main:
	mov si, hello_world

print:
	lodsb
	mov bh, 0
	mov bl, 0xf
	cmp al, 0
	je .done
	mov ah, 0x0e
	int 0x10
	jmp print

.done:
	ret

hello_world: db 'Hello World!', 0

