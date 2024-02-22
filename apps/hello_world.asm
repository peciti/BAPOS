[bits 16]

start:
	jmp test
	ret

; start:
; mov si, hello_world

test:
	mov al, 'A'
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

