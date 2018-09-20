#include <signal.h>
#include <stdio.h>

void (*signal(int sig, void (*func)(int)))(int) {
	(void)sig;
	(void)func;
	puts("signal: not implemented");
	return (void (*)(int))SIG_ERR;
}
