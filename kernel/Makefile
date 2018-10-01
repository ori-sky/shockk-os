OBJS=kernel_entry.cpp.o panic.cpp.o ports.cpp.o itoa.cpp.o screen.cpp.o page_allocator.cpp.o pager.cpp.o pit.cpp.o pic.cpp.o idt.cpp.o isr.s.o isr.cpp.o syscall.cpp.o gdt.cpp.o task.cpp.o tss.cpp.o memory.cpp.o ata.cpp.o ext2.cpp.o kb.cpp.o state.cpp.o user.s.o user_entry.cpp.o

ASMFLAGS=-f elf32
CXXFLAGS=-ffreestanding -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables -mno-sse -std=c++11 -I$(INCLUDE_PATHS) $(CXXWARNS) -g
LDFLAGS=-ffreestanding -nostdlib -T link.ld -lgcc -g

$(KERNEL_ELF): $(OBJS)
	$(TARGET_LD) $(LDFLAGS) $^ -o $@

%.cpp.o: %.cpp
	$(TARGET_CXX) $(CXXFLAGS) -c $^ -o $@

%.s.o: %.asm
	$(TARGET_ASM) $(ASMFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -fv $(OBJS) $(KERNEL_ELF)
