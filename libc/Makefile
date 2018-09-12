CRT0_OBJ=src/crt0.o
LIBC_OBJS=src/stdio.o src/string.o
CFLAGS=-Iinclude

$(CRT0_O): $(CRT0_OBJ)
	cp -v $< $@

$(LIBC_A): $(LIBC_OBJS)
	$(TARGET_AR) rcs $@ $^

src/%.o: src/%.c
	$(TARGET_CC) -c $< -o $@
