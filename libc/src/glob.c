#include <glob.h>
#include <stdio.h>

int glob(const char *restrict pattern, int flags, int(*errfunc)(const char *epath, int eerrno), glob_t *restrict pglob) {
	(void)pattern;
	(void)flags;
	(void)errfunc;
	(void)pglob;
	puts("glob: not implemented");
	return GLOB_NOSPACE;
}
void globfree(glob_t *pglob) {
	(void)pglob;
	puts("globfree: not implemented");
}
