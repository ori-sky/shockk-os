#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <kernel/cpu.h>
#include <kernel/isr.h>
#include <kernel/screen.h>
#include <kernel/state.h>
#include <kernel/syscall.h>
#include <kernel/task.h>

static const char buffer[] = "this is a file baked into the kernel, it does not exist on the disk";
static size_t pos = 0;

extern "C" int syscall_main(int command, int arg1, int arg2, int arg3, IRETState iret) {
	(void)arg3;

	switch(command) {
	case SYSCALL_COMMAND_FORK: {
		auto task = Task::Create("one.elf");
		task->next = _kernel_state.task->next;
		_kernel_state.task->next = task;
		return 0;
		return -1;
		break;
	}
	case SYSCALL_COMMAND_EXIT: {
		auto curr = _kernel_state.task;
		screen_print("exiting task ");
		screen_print(curr->exe_name);
		screen_put('\n');
		for(;;) {
			_kernel_state.task = curr->next;
			task_switch(_kernel_state.tss, curr, _kernel_state.task);
		}
		break;
	}
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
			stdin_available = false;

			// POSIX -> 11. General Terminal Interface -> Canonical Mode Input Processing
			screen_put(stdin_char, SCREEN_COLOR_USER);

			return stdin_char;
		} else {
			if(pos > sizeof(buffer)) { return EOF; }
			return buffer[pos++];
		}
	case SYSCALL_COMMAND_PUT:
		switch(arg2) {
		case STDOUT_FILENO:
			screen_put(arg1, SCREEN_COLOR_USER);
			break;
		case STDERR_FILENO:
			//screen_put(arg1, SCREEN_COLOR_ERROR); // XXX: too messy for now
			screen_put(arg1, SCREEN_COLOR_USER);
			break;
		default:
			screen_print("SYSCALL_COMMAND_PUT: unsupported fileno\n");
			break;
		}
		break;
	default:
		screen_print("unrecognized syscall command\n");
		break;
	}
	return 0;
}
