%define endl 0x0d, 0x0a
[org 0x7c00]
[bits 16]
;
; FAT12 Headers
;

jmp short start
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
start:
	cli
	xor ax, ax
	mov es, ax
	mov ds, ax

	jmp 0x00:stack
	
stack:
	sti
	mov bp, 0x7c00
	mov ss, ax
	mov sp, bp

main:
	mov [ebr_drive_number], dl
	
	; Find FAT12 Root Directory
	mov ax, [bdb_sectors_per_fat]
	mov bl, [bdb_fat_count]
	xor bh, bh
	mul bx
	add ax, [bdb_reserved_sectors]
	push ax

	mov ax, [bdb_sectors_per_fat]
	shl ax, 5
	xor dx, dx
	div word [bdb_bytes_per_sector]
	
	test dx, dx
	jz root_dir
	inc ax
	
root_dir:
	; read root dir
	mov cl, al
	pop ax
	mov dl, [ebr_drive_number]
	mov bx, buffer
	call disk_read

	; search for kernel
	xor bx, bx
	mov di, buffer

.search_kernel:
	mov si, file_kernel_bin
	mov cx, 11
	push di
	repe cmpsb
	pop di
	je .found_kernel

	add di, 32		; move to next directory
	inc bx
	cmp bx, [bdb_dir_entries_count]		; check if there are more directories
	jl .search_kernel

	jmp kernel_not_found_error

.found_kernel:
	mov ax, [di + 26]	; first cluster
	mov [kernel_cluster], ax

	; Load FAT into memory

	mov ax, [bdb_reserved_sectors]
	mov bx, buffer
	mov cl, [bdb_sectors_per_fat]
	mov dl, [ebr_drive_number]
	call disk_read

	; Read kernel and process FAT chain

	mov bx, KERNEL_LOAD_SEGMENT
	mov es, bx
	mov bx, KERNEL_LOAD_OFFSET

.load_kernel_loop:
	; Read next cluster

	mov ax, [kernel_cluster]
	add ax, 31		; should change

	mov cl, 1
	mov dl, [ebr_drive_number]
	call disk_read
	
	;this will overflow	64kB limit
	add bx, [bdb_bytes_per_sector]

	; compute location of next cluster

	mov ax, [kernel_cluster]
	mov cx, 3
	mul cx
	mov cx, 2
	div cx				; ax = Index of FAT entry
						; dx = cluster mod 2

	mov si, buffer
	add si, ax
	mov ax, [ds:si]	; read entry from fAT at index ax

	or dx, dx
	jz .even

.odd:
	shr ax, 4
	jmp .next_cluster

.even:
	and ax, 0x0FFF

.next_cluster:
	cmp ax, 0x0FF8	; end of chain
	jae .read_finish

	mov [kernel_cluster], ax	; save next cluster
	jmp .load_kernel_loop

.read_finish:
	; jump to kernel
	mov dl, [ebr_drive_number]

	mov ax, KERNEL_LOAD_SEGMENT
	mov ds, ax
	mov es, ax

	jmp KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET

	jmp wait_input ; shouldnt reach this line

	cli
	hlt
	
disk_read:
	push cx
	call lba_to_chs
	pop ax	; pop value pushed by lba_to_chs

	mov ah, 0x02
	mov dl, [ebr_drive_number]
	int 0x13

	jc read_error
	
.done:
	ret

;
;	Converts LBA address to CHS
;
lba_to_chs:
	push ax
	push dx

	xor dx, dx
	div word [bdb_sectors_per_track]
	inc dx
	mov cx, dx	; sector

	xor dx, dx
	div word [bdb_sectors_per_track]

	mov dh, dl	; head

	; cylinder stuff
	mov ch, al
	shl ah, 6
	or cl, ah
	
	; return values from stack
	pop ax
	mov dl, al
	pop ax
	ret

read_error:
	mov si, error_msg
	call print

	mov ah, 0
	int 0x16
	int 0x19
	
	ret

kernel_not_found_error:
	mov si, kernel_not_found_msg
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
kernel_not_found_msg: db 'Kernel not found! Press any key to reboot...', endl, 0
kernel_cluster:	dw 0
file_kernel_bin: db 'KERNEL  BIN'

KERNEL_LOAD_SEGMENT		equ 0x2000
KERNEL_LOAD_OFFSET			equ 0

times 510-($-$$) db 0
db 0x55, 0xaa

buffer:
