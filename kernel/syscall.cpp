#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <kernel/screen.h>
#include <kernel/syscall.h>
#include <kernel/isr.h>

static const char buffer[] = "this is a file baked into the kernel, it does not exist on the disk";
static size_t pos = 0;

extern "C" int syscall_main(int command, int arg1, int arg2, int arg3) {
	(void)arg2;
	(void)arg3;

	switch(command) {
	case SYSCALL_COMMAND_OPEN:
		return 5;
		break;
	case SYSCALL_COMMAND_GET:
		if(arg1 == STDIN_FILENO) {
			__asm__ ("sti");
			while(!stdin_available) {
				__asm__ ("hlt");
			}
			__asm__ ("cli");
			//screen_print("SYSCALL_COMMAND_GET: not implemented\n");
			//for(;;) { __asm__ ("hlt"); }
			stdin_available = false;
			return stdin_char;
		} else {
			if(pos > sizeof(buffer)) { return EOF; }
			return buffer[pos++];
		}
	case SYSCALL_COMMAND_PUT:
		screen_put(arg1, SCREEN_COLOR_USER);
		break;
	default:
		screen_print("unrecognized syscall command\n");
		break;
	}
	return 0;
}
