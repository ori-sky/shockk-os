ENTRY=kernel_main
ORIGIN=0x10000
OBJS=main.o ports.o pit.o pic.o gdt.o tss.o idt.o isr.s.o isr.o syscall.o screen.o user.s.o

ASM=nasm
CC=$(CROSS_TARGET)-gcc
LD=$(CROSS_TARGET)-gcc

ASM_FLAGS=-f elf
CFLAGS=-c -Wall -Wextra -Wpedantic -std=c99 -ffreestanding -I../include -O2
LDFLAGS=-s -e $(ENTRY) -Ttext=$(ORIGIN) -nostdlib

kernel: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o kernel.o
	objcopy -R .note -R .comment -S -O binary kernel.o kernel.bin

.PHONY: clean
clean:
	rm -fv $(OBJS)
	rm -fv kernel.o
	rm -fv kernel.bin

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@

%.s.o: %.asm
	$(ASM) $(ASM_FLAGS) $^ -o $@
