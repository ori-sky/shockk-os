ELFS=hello.elf cat.elf one.elf two.elf loop.elf
CFLAGS=-I../libc/include

.PHONY: all
all: $(ELFS)

.PHONY: install
install: all
	mkdir -pv $(SYSROOT)/bin
	cp -v $(ELFS) $(SYSROOT)/bin

%.elf: %.c $(SYSROOT)/usr/lib/libc.a
	$(TARGET_CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -fv $(ELFS)
