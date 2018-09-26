CRT0_OBJ=src/crt0.o
LIBC_OBJS=src/syscalls.o src/ctype.o src/dirent.o src/errno.o src/setjmp.o src/signal.o src/stdio.o src/stdlib.o src/string.o src/strings.o
CFLAGS=-Iinclude -I$(SHK_INCLUDE_DIR) -Wall -Wextra -Werror -g

HEADERS=$(shell find include -type f)

.PHONY: headers
headers: $(HEADERS)

$(CRT0_O): $(CRT0_OBJ) headers
	cp -v $< $@

$(LIBC_A): headers $(LIBC_OBJS)
	$(TARGET_AR) rcs $@ $(filter-out $<,$^)

src/%.o: src/%.c
	$(TARGET_CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -fv $(CRT0_OBJ)
	rm -fv $(LIBC_OBJS)
	rm -fv $(CRT0_O)
	rm -fv $(LIBC_A)
