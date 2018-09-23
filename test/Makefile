ELFS=hello.elf cat.elf
CFLAGS=-I../libc/include

.PHONY: all
all: $(ELFS)

.PHONY: install
install: all
	e2cp -v $(ELFS) $(IMAGE):/bin/

%.elf: %.c $(SYSROOT)/usr/lib/libc.a
	$(TARGET_CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -fv $(ELFS)
