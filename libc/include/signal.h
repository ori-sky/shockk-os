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
	SIG_SETMASK
};

int kill(pid_t, int);
int sigaction(int, const struct sigaction * restrict, struct sigaction * restrict);
int sigfillset(sigset_t *);
void (*signal(int, void (*)(int)))(int);

#endif
