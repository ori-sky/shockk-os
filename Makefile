IMAGE=os.img
FLOPPY_IMAGE=floppy.img

K_ENTRY=entry
K_ORIGIN=0x1000
K_C_SRCS=$(shell find lib -name "*.c" -print)
K_S_SRCS=$(shell find lib -name "*.asm" -print)
K_C_OBJS=$(K_C_SRCS:.c=.o)
K_S_OBJS=$(K_S_SRCS:.asm=.s.o)

ASM=nasm
CC=gcc
LD=gcc

ASM_FLAGS=-f bin
CFLAGS=-c -Wall --std=c99 -Iinclude -Os -ffreestanding
LDFLAGS=-s -T link.ld -e $(K_ENTRY) -Ttext=$(K_ORIGIN) -Os -nostdlib -lgcc

all: image floppy

run-qemu: image
	qemu -s -fda $(IMAGE)

clean:
	rm -f bootsector.bin
	rm -f kernel.bin
	rm -f kernel.o
	rm -f main.o
	rm -f $(K_C_OBJS)
	rm -f $(K_S_OBJS)

image: bootsector kernel
	cat bootsector.bin kernel.bin > $(IMAGE)

floppy: image
	dd bs=512 count=2820 if=/dev/zero of=$(FLOPPY_IMAGE)
	dd conv=notrunc bs=512 if=$(IMAGE) of=$(FLOPPY_IMAGE)

bootsector: bootsector.asm
	$(ASM) $(ASM_FLAGS) bootsector.asm -o bootsector.bin

kernel: $(K_C_OBJS) $(K_S_OBJS)
	$(CC) $(CFLAGS) main.c -o main.o
	$(LD) $(LDFLAGS) main.o $^ -o kernel.o
	objcopy -R .note -R .comment -S -O binary kernel.o kernel.bin

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@

%.s.o: %.asm
	$(ASM) -f elf $^ -o $@
