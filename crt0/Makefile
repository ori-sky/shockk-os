CRT0_OBJ=src/crt0.o
CFLAGS=-Iinclude -I$(SHK_INCLUDE_DIR) -Wall -Wextra -Werror -g

$(CRT0_O): $(CRT0_OBJ)
	cp -v $< $@

src/%.o: src/%.c
	$(TARGET_CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -fv $(CRT0_OBJ)
	rm -fv $(CRT0_O)
