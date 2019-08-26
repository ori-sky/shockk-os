#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <kernel/cpu.h>
#include <kernel/isr.h>
#include <kernel/panic.h>
#include <kernel/screen.h>
#include <kernel/state.h>
#include <kernel/syscall.h>
#include <kernel/task.h>

static const char buffer[] = "this is a file baked into the kernel, it does not exist on the disk";
static size_t pos = 0;

struct ret {
	int ret1;
	int ret2;
};

extern "C" ret syscall_main(int command, int arg1, int arg2, int arg3, uint32_t ebp, int /*ret2*/, int /*ret1*/, int /*pad2*/, int /*pad1*/, IRETState iret) {
	switch(command) {
	case SYSCALL_COMMAND_FORK: {
		screen_print("SYSCALL FORK\n");
		auto curr = _kernel_state.task;
		auto task = curr->Fork(ebp, iret);
		task->next = curr->next;
		curr->next = task;
		return {task->pid, 0};
	}
	case SYSCALL_COMMAND_EXEC: {
		screen_print("SYSCALL EXEC\n");
		char *path = (char *)arg1;
		if(_kernel_state.task->Exec(path)) {
			task_switch(nullptr, _kernel_state.task);
		}
		break;
	}
	case SYSCALL_COMMAND_EXIT: {
		auto curr = _kernel_state.task;
		screen_print("exiting task ");
		screen_print(curr->exe_name);
		screen_put('\n');

		curr->running = false;

		for(;;) {
			task_switch(curr, curr->next);
		}
		break;
	}
	case SYSCALL_COMMAND_GETEGID:
	case SYSCALL_COMMAND_GETEUID:
	case SYSCALL_COMMAND_GETGID:
	case SYSCALL_COMMAND_GETPID:
	case SYSCALL_COMMAND_GETPPID:
	case SYSCALL_COMMAND_GETUID:
		return {1, 0};
	case SYSCALL_COMMAND_OPEN:
		return {5, 0};
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

			return {stdin_char, 0};
		} else {
			if(pos > sizeof(buffer)) { return {EOF, 0}; }
			return {buffer[pos++], 0};
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
	case SYSCALL_COMMAND_WAITPID: {
		auto curr = _kernel_state.task;

		int pid = arg1;
		if(pid == -1) {
			bool found = false;
			while(!found) {
				task_switch(curr, curr->next);
				for(auto child = curr->child; child != nullptr; child = child->sibling) {
					if(!child->task->running) {
						found = true;
						pid = child->task->pid;
						break;
					}
				}
			}
		}
		//_kernel_state.screen << "kernel waitpid done (pid=" << pid << ", next_pid=" << _kernel_state.next_pid << ")\n";
		return {pid, 0};
	}
	default:
		kernel_panic("unrecognized syscall command");
		break;
	}
	return {0, 0};
}
