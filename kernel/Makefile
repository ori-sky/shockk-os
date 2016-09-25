#KERNEL_OBJS=kernel_entry.cpp.o panic.cpp.o ports.cpp.o itoa.cpp.o screen.cpp.o page_allocator.cpp.o pager.cpp.o pit.cpp.o pic.cpp.o idt.cpp.o isr.s.o isr.cpp.o syscall.cpp.o gdt.cpp.o tss.cpp.o memory.cpp.o user.s.o user_entry.cpp.o
OBJS=kernel_entry.cpp.o panic.cpp.o ports.cpp.o itoa.cpp.o screen.cpp.o page_allocator.cpp.o pager.cpp.o pit.cpp.o pic.cpp.o idt.cpp.o isr.s.o isr.cpp.o syscall.cpp.o gdt.cpp.o tss.cpp.o memory.cpp.o user.s.o user_entry.cpp.o

ASMFLAGS=-f elf32
CXXFLAGS=-ffreestanding -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables -mno-sse -std=c++11 -I$(INCLUDE_PATHS) $(CXXWARNS)
LDFLAGS=-ffreestanding -nostdlib -T link.ld -lgcc

$(KERNEL_ELF): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

%.cpp.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

%.s.o: %.asm
	$(ASM) $(ASMFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -fv $(OBJS) $(KERNEL_ELF)
