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

	for(;;) {
		putchar('$');
		putchar(' ');

		char buf[1024];
		size_t n;
		for(n = 0; n < 1024 && buf[n - 1] != '\n'; ++n) {
			buf[n] = getchar();
			putchar(buf[n]);
		}
		buf[n] = '\0';

		puts(buf);
	}

	return 0;
}
