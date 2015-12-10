IMAGE=os.img
FLOPPY_IMAGE=floppy.img

CROSS_TARGET=i386-elf
ASM=nasm
CC=$(CROSS_TARGET)-gcc
LD=$(CROSS_TARGET)-gcc

CWARNS=-Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wformat=2 -Winit-self -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef -Wdisabled-optimization -Wsign-conversion -Wstack-protector -Wabi -Waggregate-return -Winline -Wpadded -Wswitch-enum
CFLAGS=-Iinclude -c -ffreestanding -std=c11 -Os -fno-asynchronous-unwind-tables -mno-sse $(CWARNS)
LDFLAGS=

LOADER_OBJS=loader_entry.o ports.o a20.o panic_dummy.o page_allocator.o pager.o ata.o screen.o itoa.o
LOADER_ENTRY=loader_entry
LOADER_ORIGIN=0x1000
LOADER_PATHS=$(addprefix src/,$(LOADER_OBJS))
LOADER_LDFLAGS=-e $(LOADER_ENTRY) -Ttext=$(LOADER_ORIGIN) -nostdlib

KERNEL_OBJS=kernel_entry.o user_entry.o ports.o a20.o memory.o page_allocator.o pager.o pit.o pic.o gdt.o tss.o idt.o isr.s.o isr.o syscall.o screen.o panic.o itoa.o user.s.o pci.o ata.o
KERNEL_ENTRY=kernel_entry
KERNEL_ORIGIN=0xC0000000
KERNEL_PATHS=$(addprefix src/,$(KERNEL_OBJS))
KERNEL_ASMFLAGS=
KERNEL_LDFLAGS=-e $(KERNEL_ENTRY) -Ttext=$(KERNEL_ORIGIN) -nostdlib

.PHONY: all
all: $(FLOPPY_IMAGE)

.PHONY: run-qemu
run-qemu: $(FLOPPY_IMAGE)
	qemu -s -hda $(FLOPPY_IMAGE)

$(FLOPPY_IMAGE): $(IMAGE)
	dd bs=512 count=2820 if=/dev/zero of=$@
	dd conv=notrunc bs=512 if=$^ of=$@

$(IMAGE): bootsector.bin infosector.bin loader.bin kernel.bin
	cat $^ > $@

bootsector.bin: src/bootsector.asm
	$(ASM) -f bin $^ -o $@

infosector.bin: src/infosector.asm
	$(ASM) -f bin $^ -o $@

loader.bin: loader.o
	objcopy -R .note -R .comment -S -O binary $^ loader.bin.tmp
	dd bs=512 count=8 if=/dev/zero of=$@
	dd conv=notrunc bs=512 if=loader.bin.tmp of=$@
	rm -fv loader.bin.tmp

loader.o: $(LOADER_PATHS)
	$(LD) $(LOADER_LDFLAGS) $(LDFLAGS) $^ -o $@

kernel.bin: kernel.o
	objcopy -R .note -R .comment -S -O binary $^ $@

kernel.o: $(KERNEL_PATHS)
	$(LD) $(KERNEL_LDFLAGS) $(LDFLAGS) $^ -o $@

%.s.o: %.asm
	$(ASM) -f elf $(ASM_FLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -fv bootsector.bin $(LOADER_PATHS) loader.o loader.bin $(KERNEL_PATHS) kernel.o kernel.bin $(IMAGE) $(FLOPPY_IMAGE)
