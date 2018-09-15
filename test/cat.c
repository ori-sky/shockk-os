#include <stdio.h>

int main() {
	FILE *f = fopen("/etc/test.txt", "r");
	char buf[1024];
	fgets(buf, sizeof(buf), f);
	puts(buf);
	return 0;
}
