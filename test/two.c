#include <stdio.h>

int main() {
	for(;;) {
		printf("two\n");
		for(int i = 0; i < 100000000; ++i) {}
	}
	return 0;
}
