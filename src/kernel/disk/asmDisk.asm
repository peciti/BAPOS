bits 16

section _TEXT class=CODE

global _x86_Disk_Read
_x86_Disk_Read:
	push bp
	mov bp, sp



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
	
