#include <setjmp.h>

_Noreturn void longjmp(jmp_buf env, int val) {
	__builtin_longjmp(env, 1);
}
