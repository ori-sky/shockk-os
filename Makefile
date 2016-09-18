IMAGE=shk.img
FLOPPY_IMAGE=shk.floppy

ASM=nasm
CC=i386-elf-gcc
CXX=i386-elf-g++
LD=i386-elf-ld

CWARNS=-Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wformat=2 -Winit-self -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef -Wdisabled-optimization -Wsign-conversion -Wstack-protector -Wabi -Waggregate-return -Winline -Wpadded -Wswitch-enum
CFLAGS=-ffreestanding -nostdlib -fno-asynchronous-unwind-tables -mno-sse $(CWARNS) -std=c11 -Os -Iinclude
CXXFLAGS=-ffreestanding -nostdlib -fno-asynchronous-unwind-tables -fno-exceptions -mno-sse $(CWARNS) -std=c++11 -Os -Iinclude

LOADER_OBJS=loader_entry.cpp.o panic_dummy.cpp.o ../kernel/ports.cpp.o ../kernel/a20.cpp.o ../kernel/page_allocator.cpp.o ../kernel/pager.cpp.o ../kernel/ata.cpp.o
LOADER_PATHS=$(addprefix src/loader/,$(LOADER_OBJS))
LOADER_ENTRY=loader_entry
LOADER_ORIGIN=0x1000
LOADER_LDFLAGS=-e $(LOADER_ENTRY) -Ttext $(LOADER_ORIGIN) --build-id=none

KERNEL_OBJS=kernel_entry.cpp.o panic.cpp.o ports.cpp.o screen.cpp.o pit.cpp.o pic.cpp.o
KERNEL_PATHS=$(addprefix src/kernel/,$(KERNEL_OBJS))
KERNEL_ENTRY=kernel_entry
KERNEL_ORIGIN=0xC0000000
KERNEL_LDFLAGS=-e $(KERNEL_ENTRY) -Ttext $(KERNEL_ORIGIN)

.PHONY: all
all: $(FLOPPY_IMAGE)

.PHONY: run-qemu
run-qemu: $(FLOPPY_IMAGE)
	qemu-system-i386 -s -hda $(FLOPPY_IMAGE)

$(FLOPPY_IMAGE): $(IMAGE)
	dd bs=512 count=2820 if=/dev/zero of=$@
	dd conv=notrunc bs=512 if=$^ of=$@

$(IMAGE): bootsector.bin loader.bin kernel.elf
	cat $^ > $@

bootsector.bin: src/bootsector/bootsector.asm
	$(ASM) -f bin $^ -o $@

loader.bin: loader.flat
	dd bs=512 count=16 if=/dev/zero of=$@
	dd conv=notrunc bs=512 if=$^ of=$@

loader.flat: loader.o
	objcopy -R .note -R .comment -S -O binary $^ $@

loader.o: $(LOADER_PATHS)
	$(LD) $(LOADER_LDFLAGS) $^ -o $@

kernel.elf: $(KERNEL_PATHS)
	$(LD) $(KERNEL_LDFLAGS) $^ -o $@

%.cpp.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

%.s.o: %.asm
	$(ASM) -f elf $(ASM_FLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -fv bootsector.bin $(LOADER_PATHS) loader.o loader.flat loader.bin $(KERNEL_PATHS) kernel.elf $(IMAGE) $(FLOPPY_IMAGE)
