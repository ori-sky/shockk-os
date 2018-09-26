#include <kernel/syscall.h>
#include <unistd.h>

extern "C" void user_entry(void) __attribute__((noreturn));
void user_entry(void) {
	syscall_put(':',  STDOUT_FILENO);
	syscall_put('D',  STDOUT_FILENO);
	syscall_put('\n', STDOUT_FILENO);
	syscall_put('\n', STDOUT_FILENO);
	for(;;);
}
