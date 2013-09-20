IMAGE=os.img

BOOTSECTOR_FLAGS=-f bin
BOOTSECTOR_MAIN=bootsector.asm
BOOTSECTOR_BIN=bootsector.bin

KERNEL_CC=gcc
KERNEL_CFLAGS=-c -Wall --std=c99 -g -O0
KERNEL_LDFLAGS=
KERNEL_MAIN_SOURCE=main.c
KERNEL_MAIN_OBJECT=main.o
KERNEL_ENTRY=_main
KERNEL_ORIGIN=0x1000
KERNEL_SOURCES=
KERNEL_OBJECTS=$(KERNEL_SOURCES:.c=.o)
KERNEL_OBJECT=kernel.o
KERNEL_BIN=kernel.bin

all: bootsector kernel

run-qemu: bootsector
	qemu -fda $(BOOTSECTOR_BIN)

clean:
	rm -f $(BOOTSECTOR_BIN)
	rm -f $ ($(KERNEL_OBJECTS) $(KERNEL_OBJECT) $(KERNEL_BIN)

# TODO: combine into one file $(IMAGE)
image: bootsector kernel

bootsector:
	nasm $(BOOTSECTOR_FLAGS) $(BOOTSECTOR_MAIN) -o $(BOOTSECTOR_BIN)

kernel: $(KERNEL_OBJECTS)
	$(CC) -ffreestanding $(CFLAGS) $(KERNEL_MAIN_SOURCE) -o $(KERNEL_MAIN_OBJECT)
	ld $(KERNEL_LDFLAGS) -e $(KERNEL_ENTRY) -Ttext $(KERNEL_ORIGIN) -o $(KERNEL_OUTPUT) $(KERNEL_MAIN_OBJECT) $@
	ld $(KERNEL_LDFLAGS) -e $(KERNEL_ENTRY) -Ttext $(KERNEL_ORIGIN) -o $(KERNEL_OBJECT) $(KERNEL_MAIN_OBJECT) $@ -i
	objcopy -R .note -R .comment -S -O binary $(KERNEL_OBJECT) $(KERNEL_BIN)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

