ELFS=hello.elf cat.elf
CFLAGS=-I../libc/include

.PHONY: all
all: $(ELFS)

%.elf: %.c $(SYSROOT)/usr/lib/libc.a
	$(TARGET_CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -fv $(ELFS)
