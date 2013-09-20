IMAGE=os.img
FLOPPY_IMAGE=floppy.img

BOOTSECTOR_FLAGS=-f bin
BOOTSECTOR_MAIN=bootsector.asm
BOOTSECTOR_BIN=bootsector.bin

KERNEL_CC=gcc
KERNEL_CFLAGS=-c -Wall --std=c99 -g -O0
KERNEL_LDFLAGS=
KERNEL_MAIN_SOURCE=main.c
KERNEL_MAIN_OBJECT=main.o
KERNEL_ENTRY=main
KERNEL_ORIGIN=0x1000
KERNEL_SOURCES=
KERNEL_OBJECTS=$(KERNEL_SOURCES:.c=.o)
KERNEL_OBJECT=kernel.o
KERNEL_BIN=kernel.bin

all: image floppy

run-qemu: image
	qemu -fda $(IMAGE)

clean:
	rm -f $(BOOTSECTOR_BIN)
	rm -f $ $(KERNEL_OBJECTS) $(KERNEL_OBJECT) $(KERNEL_BIN)

image: bootsector kernel
	cat $(BOOTSECTOR_BIN) $(KERNEL_BIN) > $(IMAGE)

floppy: image
	dd bs=512 count=2820 if=/dev/zero of=$(FLOPPY_IMAGE)
	dd conv=notrunc bs=512 if=$(IMAGE) of=$(FLOPPY_IMAGE)

bootsector:
	nasm $(BOOTSECTOR_FLAGS) $(BOOTSECTOR_MAIN) -o $(BOOTSECTOR_BIN)

kernel: $(KERNEL_OBJECTS)
	$(KERNEL_CC) -ffreestanding $(CFLAGS) $(KERNEL_MAIN_SOURCE) -o $(KERNEL_MAIN_OBJECT)
	ld $(KERNEL_LDFLAGS) -e $(KERNEL_ENTRY) -Ttext $(KERNEL_ORIGIN) -o $(KERNEL_OBJECT) $(KERNEL_MAIN_OBJECT) $(KERNEL_OBJECTS)
	ld $(KERNEL_LDFLAGS) -e $(KERNEL_ENTRY) -Ttext $(KERNEL_ORIGIN) -o $(KERNEL_OBJECT) $(KERNEL_MAIN_OBJECT) $(KERNEL_OBJECTS) -i
	objcopy -R .note -R .comment -S -O binary $(KERNEL_OBJECT) $(KERNEL_BIN)

.c.o:
	$(KERNEL_CC) $(KERNEL_CFLAGS) $< -o $@

