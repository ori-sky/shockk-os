#include <stdarg.h>
#include <kernel/screen.h>
#include <kernel/syscall.h>
#include <kernel/isr.h>

extern "C" int syscall_main(int command, int arg1, int arg2, int arg3) {
	(void)arg2;
	(void)arg3;

	switch(command) {
	case SYSCALL_COMMAND_GET:
		while(!stdin_available) {
			__asm__ ("hlt");
		}
		//screen_print("SYSCALL_COMMAND_GET: not implemented\n");
		//for(;;) { __asm__ ("hlt"); }
		return 'a';
		break;
	case SYSCALL_COMMAND_PUT:
		screen_put(arg1);
		break;
	default:
		screen_print("unrecognized syscall command\n");
		break;
	}
	return 0;
}
