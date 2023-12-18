[org 0x7c00]
[bits 16]

; stack setup
stack:
	mov ax, 0
	mov ss, ax
	mov sp, 0x7c00

main:
	mov si, user_input_msg
	
	call print
	call wait_input
	jmp $

; prints a message
print:
	lodsb
	cmp al, 0
	je .done
	mov ah, 0x0e
	int 0x10

	jmp print
	
.done:
	ret

; wait for user to press a key
wait_input:
	mov ah, 0
	int 0x16
	jmp .done

.done:
	ret

user_input_msg: db 'Press any key to continue...', 0

times 510-($-$$) db 0
db 0x55, 0xaa
