LIBM_OBJS=src/math.o
CFLAGS=-Iinclude -I$(SHK_INCLUDE_DIR) -Wall -Wextra -Werror -g

HEADERS=$(shell find include -type f)

.PHONY: headers
headers: $(HEADERS)

$(LIBM_A): headers $(LIBM_OBJS)
	$(TARGET_AR) rcs $@ $(filter-out $<,$^)

src/%.o: src/%.c
	$(TARGET_CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -fv $(LIBM_OBJS)
	rm -fv $(LIBM_A)
