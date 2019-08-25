#include <stdio.h>

int main() {
	char buf[128];
	fgets(buf, 128, stdin);
	fputs(buf, stdout);
	fputc('\n', stdout);
	return 0;
}
