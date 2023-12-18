%define endl 0x0d, 0x0a
[org 0x7c00]
[bits 16]

;
; FAT12 Headers
;

jmp short main
nop
bdb_oem:					db 'MSWIN4.1'	; 8 bytes
bdb_bytes_per_sector:		dw 512
bdb_sectors_per_cluster:	db 1
bdb_reserved_sectors:		dw 1
bdb_fat_count:				db 2
bdb_dir_entries_count:		dw 224
bdb_total_sectors:			dw 2880
bdb_media_descriptor_type:	db 0F0h
bdb_sectors_per_fat:		dw 9 
bdb_sectors_per_track:		dw 18
bdb_heads:					dw 2
bdb_hidden_sectors:			dd 0
bdb_large_sector_count:		dd 0

; extended boot record

ebr_drive_number:			db 0
							db 0
ebr_signature:				db 29h
ebr_volume_id:				db 12h, 34h, 56h, 78h
ebr_volume_label:			db 'BAPOS      ' ; 11 bytes
ebr_system_id:				db 'FAT12   '	; 8 bytes

; stack setup
stack:
	mov ax, 0
	mov ss, ax
	mov sp, 0x7c00

main:
	mov si, user_input_msg
	call print
	
	call wait_input

	mov al, 1
	mov ch, 0
	mov cl, 2
	mov dh, 0
	mov dl, ebr_drive_number
	mov bx, 0x7e00
	mov es, bx
	
	pusha

	call disk_read ; load sectors into RAM
	mov si, loading_kernel_msg
	call print
	call wait_input
	
	jmp 0x7e00:0x00

disk_read:
	popa

	mov ah, 0x02
	int 0x13

	jc read_error
	
.done:
	ret

read_error:
	mov si, error_msg
	call print

	mov ah, 0
	int 0x16
	int 0x19
	
	ret
	
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

user_input_msg: db 'Press any key to continue...', endl, 0
error_msg: db 'Error! Press any key to reboot...', endl, 0 
loading_kernel_msg: db 'Loading Kernel...', endl, 0

times 510-($-$$) db 0
db 0x55, 0xaa
