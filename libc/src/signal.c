#include <signal.h>
#include <stdio.h>

int kill(pid_t pid, int sig) {
	(void)pid;
	(void)sig;
	puts("kill: not implemented");
	return -1;
}

int sigaction(int sig, const struct sigaction * restrict act, struct sigaction * restrict oact) {
	(void)sig;
	(void)act;
	(void)oact;
	puts("sigaction: not implemented");
	return -1;
}

int sigfillset(sigset_t *set) {
	(void)set;
	puts("sigfillset: not implemented");
	return -1;
}

void (*signal(int sig, void (*func)(int)))(int) {
	(void)sig;
	(void)func;
	puts("signal: not implemented");
	return (void (*)(int))SIG_ERR;
}
