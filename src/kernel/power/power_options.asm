bits 16

section _TEXT class=CODE

global _x86_Shutdown
_x86_Shutdown:

; check if APM is available
mov ah, 0x53
mov al, 0x00
xor bx, bx
int 15h
jc apm_not_available_error

; connect to APM interface
mov ah, 0x53
mov al, 0x01
xor bx, bx
int 0x15
jc interface_error

; set APM version
mov ah, 0x53
mov al, 0x0e
mov bx, 0x0000
mov ch, 0x01
mov cl, 0x01
int 0x15
jc version_error

continue:
; Enable power management for all devices
mov ah, 0x53
mov al, 0x08
mov bx, 0x0001
mov cx, 0x0001
int 0x15
jc power_mode_error

; Set the power state for all devices
mov ah, 0x53
mov al, 0x07
mov bx, 0x0001
mov cx, 0x03
int 0x15
jc APM_error

; shouldnt reach this i think idk
jmp $

APM_error:
	mov si, apm_error_message
	call print
	
	ret

apm_not_available_error:
	mov si, apm_is_not_available_message
	call print

	ret

power_mode_error:
	mov si, apm_could_not_enable_power_mode
	call print

	ret
	
interface_error:
	mov si, apm_cannot_connect_to_interface
	call print

	ret

version_error:
	mov si, apm_version_error
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
apm_is_not_available_message: db 'APM is nott available on this machine...'
apm_cannot_connect_to_interface: db 'Could not connect to APM interface...'
apm_version_error: db 'There was an error settings APM version...'
apm_could_not_enable_power_mode: db 'Could not enable device power mode...'
