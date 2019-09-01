#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void * memcpy(void * restrict s1, const void * restrict s2, size_t n) {
	//printf("\ndebug memcpy %u %u %u\n", (unsigned int)s1, (unsigned int)s2, (unsigned int)n);
	char *sz1 = s1;
	const char *sz2 = s2;
	for(size_t i = 0; i < n; ++i) {
		sz1[i] = sz2[i];
	}
	return s1;
}

char * memmove(void *s1, const void *s2, size_t n) {
	char *sz1 = s1;
	const char *sz2 = s2;
	for(size_t i = 0; i < n; ++i) {
		if(s1 < s2) {
			sz1[i] = sz2[i];
		} else if(s1 > s2) {
			sz1[n - 1 - i] = sz2[n - 1 - i];
		}
	}
	return s1;
}

char * strcpy(char * restrict s1, const char * restrict s2) {
	size_t n;
	for(n = 0; s2[n] != '\0'; ++n) {
		s1[n] = s2[n];
	}
	s1[n] = '\0';
	return s1;
}

char * strdup(const char *s) {
	size_t len = strlen(s);
	char *dup = malloc(len + 1);
	memcpy(dup, s, len + 1);
	return dup;
}

int memcmp(const void *s1, const void *s2, size_t n) {
	const char *sz1 = s1;
	const char *sz2 = s2;
	for(size_t i = 0; i < n; ++i) {
		if(sz1[i] < sz2[i]) { return -1; }
		if(sz1[i] > sz2[i]) { return  1; }
	}
	return 0;
}

int strcmp(const char *s1, const char *s2) {
	for(size_t i = 0; s1[i] != '\0'; ++i) {
		if(s1[i] < s2[i]) { return -1; }
		if(s1[i] > s2[i]) { return  1; }
	}
	return 0;
}

char * strchr(const char *s, int c) {
	for(size_t i = 0; s[i] != '\0'; ++i) {
		if(s[i] == c) { return (char *)&s[i]; }
	}
	return NULL;
}

size_t strcspn(const char *s1, const char *s2) {
	size_t n;
	for(n = 0; s1[n] != '\0'; ++n) {
		if(s1[n] == s2[n]) { break; }
	}
	return n;
}

char * strpbrk(const char *s1, const char *s2) {
	for(size_t i = 0; s2[i] != '\0'; ++i) {
		for(size_t n = 0; s2[n] != '\0'; ++n) {
			if(s1[i] == s2[n]) { return (char *)&s1[i]; }
		}
	}
	return NULL;
}

char * strrchr(const char *s, int c) {
	bool found = false;
	size_t loc = 0;

	for(size_t i = 0; s[i] != '\0'; ++i) {
		if(s[i] == c) {
			found = true;
			loc = i;
		}
	}

	return found ? (char *)&s[loc] : NULL;
}

size_t strspn(const char *s1, const char *s2) {
	size_t n;
	for(n = 0; s1[n] != '\0' && s2[n] != '\0'; ++n) {
		if(s1[n] != s2[n]) { break; }
	}
	return n;
}

char * strstr(const char *s1, const char *s2) {
	(void)s1;
	(void)s2;
	puts("strstr: not implemented");
	return NULL;
}

char * strtok(char * restrict s1, const char * restrict s2) {
	(void)s1;
	(void)s2;
	puts("strtok: not implemented");
	return NULL;
}

void * memset(void *s, int c, size_t n) {
	unsigned char *sz = s;
	for(size_t i = 0; i < n; ++i) {
		sz[i] = (unsigned char)c;
	}
	return s;
}

char * strerror(int errnum) {
	enum _errno_e e = errnum;
	switch(e) {
	case EACCES:
		return "EACCES";
	case EAFNOSUPPORT:
		return "EAFNOSUPPORT";
	case EAGAIN:
		return "EAGAIN";
	case EBADF:
		return "EBADF";
	case EBUSY:
		return "EBUSY";
	case ECHILD:
		return "ECHILD";
	case EEXIST:
		return "EEXIST";
	case EINTR:
		return "EINTR";
	case EINVAL:
		return "EINVAL";
	case EIO:
		return "EIO";
	case EISDIR:
		return "EISDIR";
	case ELOOP:
		return "ELOOP";
	case ENAMETOOLONG:
		return "ENAMETOOLONG";
	case ENOENT:
		return "ENOENT";
	case ENOEXEC:
		return "ENOEXEC";
	case ENOMEM:
		return "ENOMEM";
	case ENOSYS:
		return "ENOSYS";
	case ENOTTY:
		return "ENOTTY";
	case ENOTDIR:
		return "ENOTDIR";
	case ERANGE:
		return "ERANGE";
	case ESPIPE:
		return "ESPIPE";
	case EWOULDBLOCK:
		return "EWOULDBLOCK";
	}
	return "unknown errnum";
}

size_t strlen(const char *s) {
	size_t n;
	for(n = 0; s[n] != '\0'; ++n) {}
	return n;
}

char * strsignal(int signum) {
	switch(signum) {
	default:
		return "unknown signum";
	}
}
