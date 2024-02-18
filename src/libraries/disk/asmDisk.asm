bdb_sectors_per_track: dw 18
bdb_heads: db 2
[bits 16]

section _TEXT class=CODE
global _x86_Disk_Read
_x86_Disk_Read:
	sti
	push bp
	mov bp, sp

	mov ax, [bp + 4]
	mov cx, [bp + 6]
	mov bx, [bp + 8]

	push es
	push dx
	mov dx, [bp + 14]
	mov es, dx
	pop dx

	call disk_read

	pop es
	mov sp, bp
	pop bp
	ret

global _x86_Disk_Reset
_x86_Disk_Reset:
	push bp
	mov bp, sp

	mov ah, 0
	mov dl, [bp+4]
	stc

	int 0x13
	jc reset_error

	mov cx,0
	mov bx, [bp+6]
	mov [bx], cx
	jmp end_reset

reset_error:
	mov bx, [bp+6]
	mov cx, 1
	mov [bx], cx

end_reset:
	mov sp, bp
	pop bp
	ret

disk_read:
	push cx
	call lba_to_chs
	pop ax

	mov ah, 0x02
	int 0x13

lba_to_chs:
	push ax
	push dx
	
	 xor dx, dx                          ; dx = 0
	 div word [bdb_sectors_per_track]     ; ax = LBA / SectorsPerTrack
	                                            ; dx = LBA % SectorsPerTrack
	
	 inc dx                              ; dx = (LBA % SectorsPerTrack + 1) = sector
	 mov cx, dx                          ; cx = sector
	
	 xor dx, dx                          ; dx = 0
	 div word [bdb_heads]                ; ax = (LBA / SectorsPerTrack) / Heads = cylinder
	                                            ; dx = (LBA / SectorsPerTrack) % Heads = head
	 mov dh, dl                          ; dh = head
	 mov ch, al                          ; ch = cylinder (lower 8 bits)
	 shl ah, 6
	 or cl, ah                           ; put upper 2 bits of cylinder in CL
	
	 pop ax
	 mov dl, al                          ; restore DL
	 pop ax
	 ret
