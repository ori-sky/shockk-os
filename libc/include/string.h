#ifndef _STRING_H
#define _STRING_H 1

#include <stddef.h>

char * strcpy(char * restrict, const char * restrict);

int strcmp(const char *, const char *);

char * strchr(const char *, int);
char * strstr(const char *, const char *);

char * strerror(int);
size_t strlen(const char *);
char * strsignal(int);

#endif
