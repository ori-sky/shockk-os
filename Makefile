IMAGE=os.img
FLOPPY_IMAGE=floppy.img

BOOTSECTOR_FLAGS=-f bin
BOOTSECTOR_MAIN=bootsector.asm
BOOTSECTOR_BIN=bootsector.bin

ASM_FLAGS=-f bin
ASM_SOURCES=idt.asm
ASM_BINS=$(ASM_SOURCES:.asm=.bin)

KERNEL_CC=gcc
KERNEL_CFLAGS=-m32 -c -Wall --std=c99 -g -O0
KERNEL_LDFLAGS=-melf_i386 -T link.ld
KERNEL_MAIN_CFLAGS=-ffreestanding -falign-functions=512
KERNEL_MAIN_SOURCE=main.c
KERNEL_MAIN_OBJECT=main.o
KERNEL_ENTRY=kernel_entry
KERNEL_ORIGIN=0x2000
KERNEL_SOURCES=ports.c pic.c screen.c
KERNEL_OBJECTS=$(KERNEL_SOURCES:.c=.o)
KERNEL_OBJECT=kernel.o
KERNEL_BIN=kernel.bin

all: image floppy

run-qemu: image
	qemu -s -fda $(IMAGE)

clean:
	rm -f $(BOOTSECTOR_BIN)
	rm -f $(KERNEL_BIN)
	rm -f $(KERNEL_OBJECT)
	rm -f $(KERNEL_OBJECTS)
	rm -f $(KERNEL_MAIN_OBJECT)

image: bootsector $(ASM_BINS) kernel
	cat $(BOOTSECTOR_BIN) $(ASM_BINS) $(KERNEL_BIN) > $(IMAGE)

floppy: image
	dd bs=512 count=2820 if=/dev/zero of=$(FLOPPY_IMAGE)
	dd conv=notrunc bs=512 if=$(IMAGE) of=$(FLOPPY_IMAGE)

bootsector:
	nasm $(BOOTSECTOR_FLAGS) $(BOOTSECTOR_MAIN) -o $(BOOTSECTOR_BIN)

asm: $(ASM_BINS)

kernel: $(KERNEL_OBJECTS)
	$(KERNEL_CC) $(KERNEL_MAIN_CFLAGS) $(KERNEL_CFLAGS) $(KERNEL_MAIN_SOURCE) -o $(KERNEL_MAIN_OBJECT)
	ld $(KERNEL_LDFLAGS) -e $(KERNEL_ENTRY) -Ttext $(KERNEL_ORIGIN) -o $(KERNEL_OBJECT) $(KERNEL_MAIN_OBJECT) $^
	objcopy -R .note -R .comment -S -O binary $(KERNEL_OBJECT) $(KERNEL_BIN)

%.bin: %.asm
	nasm $(ASM_FLAGS) $^ -o $@

%.o: %.c
	$(KERNEL_CC) $(KERNEL_CFLAGS) $^ -o $@
