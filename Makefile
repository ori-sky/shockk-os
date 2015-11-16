IMAGE=os.img
FLOPPY_IMAGE=floppy.img

K_ENTRY=entry
K_ORIGIN=0x1000
K_C_SRCS=$(shell find lib -name "*.c" -print)
K_S_SRCS=$(shell find lib -name "*.asm" -print)
K_C_OBJS=$(K_C_SRCS:.c=.o)
K_S_OBJS=$(K_S_SRCS:.asm=.s.o)

ASM=nasm
CC=$(CROSS_TARGET)-gcc
LD=$(CROSS_TARGET)-ld

ASM_FLAGS=-f bin
CFLAGS=-c -Wall --std=c99 -Iinclude -ffreestanding
LDFLAGS=-s -e $(K_ENTRY) -Ttext=$(K_ORIGIN) -nostdlib

all: image floppy

run-qemu: floppy
	qemu -s -fda $(FLOPPY_IMAGE)

clean:
	rm -fv bootsector.bin
	rm -fv kernel.bin
	rm -fv kernel.o
	rm -fv main.o
	rm -fv $(K_C_OBJS)
	rm -fv $(K_S_OBJS)

image: bootsector kernel
	cat bootsector.bin kernel.bin > $(IMAGE)

floppy: image
	dd bs=512 count=2820 if=/dev/zero of=$(FLOPPY_IMAGE)
	dd conv=notrunc bs=512 if=$(IMAGE) of=$(FLOPPY_IMAGE)

bootsector: bootsector.asm
	$(ASM) $(ASM_FLAGS) bootsector.asm -o bootsector.bin

#kernel: main.o $(K_C_OBJS) $(K_S_OBJS)
kernel: main.o
	$(LD) $(LDFLAGS) $^ -o kernel.o
	objcopy -R .note -R .comment -S -O binary kernel.o kernel.bin

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@

%.s.o: %.asm
	$(ASM) -f elf $^ -o $@
