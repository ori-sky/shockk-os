#include <kernel/screen.h>
#include <kernel/task.h>

void task_entry(void) {
	screen_print("spawned new task\n");
	for(;;) {
		__asm__ ("hlt");
	}
}
