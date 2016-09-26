OBJS=multiboot.s.o loader_entry.cpp.o panic_dummy.cpp.o ../kernel/ports.cpp.o ../kernel/page_allocator.cpp.o ../kernel/pager.cpp.o ../kernel/ata.cpp.o ../kernel/mbr.cpp.o ../kernel/itoa.cpp.o

ASMFLAGS=-f elf32
CXXFLAGS=-ffreestanding -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables -mno-sse -std=c++11 -I$(INCLUDE_PATHS) $(CXXWARNS)
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
