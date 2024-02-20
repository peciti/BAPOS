[bits 16]
[org 0x0]

start:
 jmp test

; start:
; mov si, hello_world

test:
	mov al, 65
	mov ah, 0x0e
	int 0x10
	jmp return

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

