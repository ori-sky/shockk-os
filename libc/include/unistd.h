#ifndef _UNISTD_H
#define _UNISTD_H 1

#include <stddef.h>
#include <sys/types.h>

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

ssize_t write(int, const void *, size_t);

#endif
