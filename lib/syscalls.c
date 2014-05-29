#include <syscalls.h>

const syscall_t syscalls[20] =
{
	0,
	sc_exit,
	sc_close,
	sc_execve,
	sc_fork,
	sc_fstat,
	sc_getpid,
	sc_isatty,
	sc_kill,
	sc_link,
	sc_lseek,
	sc_open,
	sc_read,
	sc_sbrk,
	sc_stat,
	sc_times,
	sc_unlink,
	sc_wait,
	sc_write,
};

int sc_exit(struct syscall sc)
{
	return -1;
}

int sc_close(struct syscall sc)
{
	return -1;
}

int sc_execve(struct syscall sc)
{
	return -1;
}

int sc_fork(struct syscall sc)
{
	return -1;
}

int sc_fstat(struct syscall sc)
{
	return -1;
}

int sc_getpid(struct syscall sc)
{
	return -1;
}

int sc_isatty(struct syscall sc)
{
	return -1;
}

int sc_kill(struct syscall sc)
{
	return -1;
}

int sc_link(struct syscall sc)
{
	return -1;
}

int sc_lseek(struct syscall sc)
{
	return -1;
}

int sc_open(struct syscall sc)
{
	return -1;
}

int sc_read(struct syscall sc)
{
	return -1;
}

int sc_sbrk(struct syscall sc)
{
	return -1;
}

int sc_stat(struct syscall sc)
{
	return -1;
}

int sc_times(struct syscall sc)
{
	return -1;
}

int sc_unlink(struct syscall sc)
{
	return -1;
}

int sc_wait(struct syscall sc)
{
	return -1;
}

int sc_write(struct syscall sc)
{
	return -1;
}
