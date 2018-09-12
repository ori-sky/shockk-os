#include <stdio.h>

int main() {
	fprintf(stderr, "hello stderr\n");
	printf("a");
	putchar('\n');
	printf("b");
	printf("\n");
	printf("c\nd");
	printf("\n");
	printf("hello world\n");
	printf("dlrow olleh\n");
	for(unsigned long long i = 0; i<(0xE + 0xA); ++i) {
		printf("i = %llu\n", i);
	}
	return 0;
}
