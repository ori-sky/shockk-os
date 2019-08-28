#ifndef _SIGNAL_H
#define _SIGNAL_H 1

#include <sys/types.h>

typedef int sig_atomic_t;
typedef int sigset_t;

enum {
	SIG_DFL,
	SIG_ERR,
	SIG_IGN
};

enum {
	SIGABRT,
	SIGALRM,
	SIGCHLD,
	SIGCONT,
	SIGHUP,
	SIGINT,
	SIGPIPE,
	SIGQUIT,
	SIGTERM,
	SIGTSTP,
	SIGTTIN,
	SIGTTOU,
	SIGUSR1,
	SIGUSR2,
	SIGVTALRM,
	SIGXCPU,
	SIGXFSZ,
	NSIG
};

union sigval {
	int sival_int;
	void *sival_ptr;
};

typedef struct {
	int si_signo;
	int si_code;
	int si_errno;
	pid_t si_pid;
	uid_t si_uid;
	void *si_addr;
	int si_status;
	long si_band;
	union sigval si_value;
} siginfo_t;

struct sigaction {
	void (*sa_handler)(int);
	sigset_t sa_mask;
	int sa_flags;
	void (*sa_sigaction)(int, siginfo_t *, void *);
};

enum {
	SIG_BLOCK,
	SIG_UNBLOCK,
	SIG_SETMASK
};

enum {
	SA_NOCLDSTOP,
	SA_ONSTACK,
	SA_RESETHAND,
	SA_RESTART,
	SA_SIGINFO,
	SA_NOCLDWAIT,
	SA_NODEFER,
	SS_ONSTACK,
	SS_DISABLE,
	MINSIGSTKSZ,
	SIGSTKSZ
};

void (*signal(int, void (*)(int)))(int);

int kill(pid_t, int);
int raise(int);

int sigaction(int, const struct sigaction * restrict, struct sigaction * restrict);
int sigemptyset(sigset_t *);
int sigfillset(sigset_t *);
int sigprocmask(int, const sigset_t * restrict, sigset_t * restrict);
int sigsuspend(const sigset_t *);

#endif
