#include <stdio.h>

char *optarg;
int opterr;
int optind;
int optopt;

int getopt(int argc, char * const argv[], const char *optstring) {
	(void)argc;
	(void)argv;
	(void)optstring;
	puts("getopt: not implemented");
	return -1;
}
