#include <signal.h>
#include <stdio.h>

void (*signal(int sig, void (*func)(int)))(int) {
	(void)sig;
	(void)func;
	puts("signal: not implemented");
	return (void (*)(int))SIG_ERR;
}

int kill(pid_t pid, int sig) {
	(void)pid;
	(void)sig;
	puts("kill: not implemented");
	return -1;
}

int raise(int sig) {
	(void)sig;
	puts("raise: not implemented");
	return 1;
}

int sigaction(int sig, const struct sigaction * restrict act, struct sigaction * restrict oact) {
	(void)sig;
	(void)act;
	(void)oact;
	puts("sigaction: not implemented");
	return -1;
}

int sigemptyset(sigset_t *set) {
	(void)set;
	puts("sigemptyset: not implemented");
	return -1;
}

int sigfillset(sigset_t *set) {
	(void)set;
	puts("sigfillset: not implemented");
	return -1;
}

int sigprocmask(int how, const sigset_t * restrict set, sigset_t * restrict oset) {
	(void)how;
	(void)set;
	(void)oset;
	puts("sigprocmask: not implemented");
	return -1;
}

int sigsuspend(const sigset_t *sigmask) {
	(void)sigmask;
	puts("sigsuspend: not implemented");
	return -1;
}
