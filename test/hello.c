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
	for(unsigned long long i = 0; i < 8; ++i) {
		printf("i = %llu\n", i);
	}
	printf("there should be a newline at the end of this\n");
	puts("and this");
	fputs("is there?\n", stdout);
	putchar('$');
	return 0;
}
