ELFS=hello.elf
CFLAGS=-I../libc/include

.PHONY: all
all: $(ELFS)

%.elf: %.c
	$(TARGET_CC) $(CFLAGS) $< -o $@
