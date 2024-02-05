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
	mov si, press_to_continue
	call print

wait_input:
	mov ah, 0x0
	int 0x16
	mov si, hello_world
	call print

hello_world: db 'Hello World!', 0
press_to_continue: db 'Press any key to continue...', 0
