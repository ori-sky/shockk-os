OBJS=bootstrap.s.o

ASMFLAGS=-f bin

$(MBR_BIN): $(OBJS)
	cat $^ > $@

%.s.o: %.asm
	$(TARGET_ASM) $(ASMFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -fv $(OBJS) $(MBR_BIN)
