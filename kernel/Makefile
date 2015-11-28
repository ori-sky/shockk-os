ENTRY=kernel_main
ORIGIN=0x10000
OBJS=main.o ports.o a20.o memory.o page_allocator.o pager.o pit.o pic.o gdt.o tss.o idt.o isr.s.o isr.o syscall.o screen.o panic.o itoa.o user.s.o pci.o

ASM=nasm
CC=$(CROSS_TARGET)-gcc
LD=$(CROSS_TARGET)-gcc

K_ASM_FLAGS=-f elf
K_CFLAGS=-I../include -c -ffreestanding -fno-asynchronous-unwind-tables -std=c11 -O2 -mno-sse -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wformat=2 -Winit-self -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef -Wdisabled-optimization -Wsign-conversion -Wstack-protector -Wabi -Waggregate-return -Winline -Wpadded -Wswitch-enum
K_LDFLAGS=-s -e $(ENTRY) -Ttext=$(ORIGIN) -nostdlib
ASM_FLAGS=
CFLAGS=
LDFLAGS=

kernel: $(OBJS)
	$(LD) $(K_LDFLAGS) $(LDFLAGS) $^ -o kernel.o
	objcopy -R .note -R .comment -S -O binary kernel.o kernel.bin

.PHONY: clean
clean:
	rm -fv $(OBJS)
	rm -fv kernel.o
	rm -fv kernel.bin

%.o: %.c
	$(CC) $(K_CFLAGS) $(CFLAGS) $^ -o $@

%.s.o: %.asm
	$(ASM) $(K_ASM_FLAGS) $(ASM_FLAGS) $^ -o $@
