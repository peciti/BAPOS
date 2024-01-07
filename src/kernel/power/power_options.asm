bits 16

section _TEXT class=CODE

global _x86_Shutdown
_x86_Shutdown:

; Set the power state for all devices
mov ah, 0x53
mov al, 0x07
mov bx, 0x0001
mov cx, 0x0003
int 0x15
jc APM_error

; shouldnt reach this i think idk
jmp $

APM_error:
	mov si, apm_error_message
	call print
	
	ret

print:
	lodsb
	cmp al, 0
	je .done
	mov ah, 0x0e
	int 0x10
	jmp print

.done:
	ret
	
apm_error_message: db 'There was an error while shutting down...'
