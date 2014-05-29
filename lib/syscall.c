#include <syscall.h>
#include <syscalls.h>

void syscall_handler(struct cpu_state *state)
{
	struct syscall sc;
	sc.num = state->eax;
	sc.params[0] = state->ebx;
	sc.params[1] = state->ecx;
	sc.params[2] = state->edx;
	sc.params[3] = state->esi;
	sc.params[4] = state->edi;
	sc.params[5] = (int)state->ebp;

	syscall_t call = syscalls[sc.num];
	if(sc.num >= sizeof(syscalls) / sizeof(syscall_t) || call == 0) return;

	state->eax = call(sc);
}
