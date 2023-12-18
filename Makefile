
ASM=nasm

SRC=src
BUILD=build

BOOTLOADER_SRC=$(SRC)/bootloader
KERNEL_SRC=$(SRC)/kernel

all: bootloader kernel floppy
.PHONY : all

#
#	Bootloader
#

bootloader:
	$(ASM) $(BOOTLOADER_SRC)/boot.asm -o $(BUILD)/boot.bin

#
#	Kernel
#

kernel:
	$(ASM) $(KERNEL_SRC)/kernel.asm -o $(BUILD)/kernel.bin

#
#	Floppy Image
#

floppy:
	dd bs=512 count=2880 if=/dev/zero of=$(BUILD)/floppy.img
	sudo mkfs.vfat -F12 $(BUILD)/floppy.img
	mcopy -i $(BUILD)/floppy.img $(BUILD)/kernel.bin "::kernel.bin"
	dd if=$(BUILD)/boot.bin of=$(BUILD)/floppy.img seek=0 count=1 conv=notrunc
	
