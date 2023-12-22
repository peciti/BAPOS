
ASM=nasm

SRC=src
BUILD=build

BOOTLOADER_SRC=$(SRC)/bootloader
KERNEL_SRC=$(SRC)/kernel

#
#	Floppy Image
#

floppy: $(BUILD)/floppy.img
$(BUILD)/floppy.img: bootloader kernel
	dd bs=512 count=2880 if=/dev/zero of=$(BUILD)/floppy.img	
	sudo mkfs.fat -F12 -n "BAPOS" $(BUILD)/floppy.img
	dd if=$(BUILD)/boot.bin of=$(BUILD)/floppy.img conv=notrunc
	mcopy -i $(BUILD)/floppy.img $(BUILD)/kernel.bin "::kernel.bin"


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

