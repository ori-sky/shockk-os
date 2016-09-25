#SRCS=multiboot.asm entry.cpp panic_dummy.cpp ../kernel/ports.cpp ../kernel/page_allocator.cpp ../kernel/pager.cpp ../kernel/ata.cpp
OBJS=multiboot.s.o loader_entry.cpp.o

ASMFLAGS=-f elf32
CXXFLAGS=-ffreestanding -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables -mno-sse -std=c++11 $(CXXWARNS)
LDFLAGS=-ffreestanding -nostdlib -T link.ld -lgcc

$(LOADER_ELF): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

%.cpp.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

%.s.o: %.asm
	$(ASM) $(ASMFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -fv $(OBJS) $(LOADER_ELF)
