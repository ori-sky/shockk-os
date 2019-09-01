#include <kernel/elf.h>
#include <kernel/panic.h>
#include <kernel/screen.h>
#include <kernel/state.h>
#include <kernel/task.h>
#include <kernel/user.h>

void task_entry(Task *task) {
	__asm__ ("sti");

	//_kernel_state.screen << "spawned new task " << task->exe_name << '\n';

	_kernel_state.pager->Enable(task->context);
	user_enter(task->entry, task->stack);

	for(;;) { __asm__ ("hlt"); } // unreachable
}
