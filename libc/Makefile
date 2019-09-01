LIBC_OBJS=src/syscalls.o src/ctype.o src/dirent.o src/errno.o src/fnmatch.o src/getopt.o src/glob.o src/libgen.o src/malloc.o src/poll.o src/pwd.o src/setjmp.o src/signal.o src/stdio.o src/stdlib.o src/string.o src/strings.o src/termios.o src/sys/time.o src/sys/times.o src/unistd.o
CFLAGS=-Iinclude -I$(SHK_INCLUDE_DIR) -Wall -Wextra -Werror -g

HEADERS=$(shell find include -type f)

.PHONY: headers
headers: $(HEADERS)

$(LIBC_A): headers $(LIBC_OBJS)
	$(TARGET_AR) rcs $@ $(filter-out $<,$^)

src/%.o: src/%.c
	$(TARGET_CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -fv $(LIBC_OBJS)
	rm -fv $(LIBC_A)
