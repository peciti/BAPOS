[bits 16]
.entry:
	sti
	jmp start

start:
	cli
	mov ax, ds
	mov ss, ax
	mov sp, 0x1000
	sti

	jmp main
	ret

main:
 mov si, hello_world

print:
	lodsb
	cmp al, 0
	je .done
	mov ah, 0x0e
	int 0x10
	jmp print

.done:
	jmp return

return:
	ret

hello_world: db 'Hello World!', 0

