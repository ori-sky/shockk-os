OBJS=bootsector.s.o stage1.s.o

ASMFLAGS=-f bin

$(SHKBOOT_BIN): $(OBJS)
	cat $^ > $@

%.s.o: %.asm
	$(TARGET_ASM) $(ASMFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -fv $(OBJS) $(SHKBOOT_BIN)
