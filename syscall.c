#include "screen.h"

int syscall_main(int command) {
	screen_put(command);
	return -1;
}
