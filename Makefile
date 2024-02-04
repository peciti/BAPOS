
ASM=nasm
CC16=/usr/bin/watcom/binl/wcc
CFLAGS16=-s -wx -ms -zl -zq
LD16=/usr/bin/watcom/binl/wlink
ASM_FLAGS=-f obj

SRC=src
BUILD=build

BOOTLOADER_SRC=$(SRC)/bootloader
KERNEL_SRC=$(SRC)/kernel

all: clean always floppy snake_floppy bootloader kernel
#
#	Floppy Image
#

floppy: $(BUILD)/floppy.img
$(BUILD)/floppy.img: bootloader kernel
	dd bs=512 count=2880 if=/dev/zero of=$(BUILD)/floppy.img	
	sudo mkfs.fat -F 12 -n "BAPOS" $(BUILD)/floppy.img
	dd if=$(BUILD)/boot.bin of=$(BUILD)/floppy.img conv=notrunc
	mcopy -i $(BUILD)/floppy.img $(BUILD)/kernel.bin "::kernel.bin"
	mcopy -i $(BUILD)/floppy.img apps/hello_world.bin "::hello.bin"

snake_floppy: $(BUILD)/snake_floppy.img
$(BUILD)/snake_floppy.img:
	dd bs=512 count=2880 if=/dev/zero of=$(BUILD)/snake_floppy.img	
		sudo mkfs.fat -F 12 -n "SNAKE" $(BUILD)/snake_floppy.img
		dd if=apps/snake.bin of=$(BUILD)/snake_floppy.img conv=notrunc

#
#	Bootloader
#

bootloader:
	$(ASM) $(BOOTLOADER_SRC)/boot.asm -o $(BUILD)/boot.bin

#
#	Kernel
#

kernel:
	$(ASM) $(ASM_FLAGS) -o $(BUILD)/kernel/asm/kernel.obj $(KERNEL_SRC)/kernel.asm
	$(ASM)  $(ASM_FLAGS) -o $(BUILD)/kernel/asm/print.obj $(SRC)/libraries/stdio/print.asm
	$(ASM) $(ASM_FLAGS) -o $(BUILD)/kernel/asm/input.obj $(SRC)/libraries/stdio/input.asm
	$(ASM) $(ASM_FLAGS) -o $(BUILD)/kernel/asm/power.obj $(SRC)/libraries/power/power_options.asm
	$(ASM) $(ASM_FLAGS) -o $(BUILD)/kernel/asm/disk.obj $(SRC)/libraries/disk/asmDisk.asm
	$(CC16) $(CFLAGS16) -fo=$(BUILD)/kernel/c/kernel.obj $(KERNEL_SRC)/kernel.c
	$(CC16) $(CFLAGS16) -fo=$(BUILD)/kernel/c/stdio.obj $(SRC)/libraries/stdio/stdio.c
	$(LD16) NAME $(BUILD)/kernel.bin FILE \{$(BUILD)/kernel/asm/kernel.obj $(BUILD)/kernel/asm/power.obj  $(BUILD)/kernel/asm/print.obj $(BUILD)/kernel/asm/disk.obj $(BUILD)/kernel/asm/input.obj $(BUILD)/kernel/c/kernel.obj $(BUILD)/kernel/c/stdio.obj \} OPTION MAP=$(BUILD)/kernel.map @$(KERNEL_SRC)/linker.lnk

always:
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/kernel
	mkdir -p $(BUILD)/kernel/asm
	mkdir -p $(BUILD)/kernel/c

clean:
	rm -rf $(BUILD)/*
