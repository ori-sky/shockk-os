IMAGE=os.img
FLOPPY_IMAGE=floppy.img

K_ENTRY=entry
K_ORIGIN=0x1000
K_OBJS=main.o ports.o pic.o idt.o isr.s.o

ASM=nasm
CC=$(CROSS_TARGET)-gcc
LD=$(CROSS_TARGET)-ld

ASM_FLAGS=-f elf
CFLAGS=-c -Wall -Wextra -Wpedantic -std=c99 -ffreestanding -Iinclude -O2
LDFLAGS=-s -e $(K_ENTRY) -Ttext=$(K_ORIGIN) -nostdlib

all: image floppy

run-qemu: floppy
	qemu -s -fda $(FLOPPY_IMAGE)

clean:
	rm -fv bootsector.bin
	rm -fv $(K_OBJS)
	rm -fv kernel.o
	rm -fv kernel.bin
	rm -fv os.img
	rm -fv floppy.img

image: bootsector kernel
	cat bootsector.bin kernel.bin > $(IMAGE)

floppy: image
	dd bs=512 count=2820 if=/dev/zero of=$(FLOPPY_IMAGE)
	dd conv=notrunc bs=512 if=$(IMAGE) of=$(FLOPPY_IMAGE)

bootsector: bootsector.asm
	$(ASM) -f bin $^ -o bootsector.bin

#kernel: main.o $(K_C_OBJS) $(K_S_OBJS)
kernel: $(K_OBJS)
	$(LD) $(LDFLAGS) $^ -o kernel.o
	objcopy -R .note -R .comment -S -O binary kernel.o kernel.bin

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@

%.s.o: %.asm
	$(ASM) $(ASM_FLAGS) $^ -o $@
