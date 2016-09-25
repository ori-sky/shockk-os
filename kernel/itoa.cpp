#include <kernel/itoa.h>

int itoa(int value, char *str, int radix) {
	char tmp[16];
	char *ptr = tmp;

	int sign = radix == 10 && value < 0;
	unsigned int v = sign ? (unsigned int)-value : (unsigned int)value;

	while(v || ptr == tmp) {
		int i = (int)v % radix;
		v /= (unsigned int)radix;
		if(i < 10) {
			*ptr++ = i + '0';
		} else {
			*ptr++ = i + 'a' - 10;
		}
	}

	int len = ptr - tmp;

	if(sign) {
		*str++ = '-';
		++len;
	}

	while(ptr > tmp) { *str++ = *--ptr; }
	return len;
}

int uitoa(unsigned int value, char *str, unsigned int radix) {
	char tmp[16];
	char *ptr = tmp;

	while(value || ptr == tmp) {
		unsigned int i = value % radix;
		value /= radix;
		if(i < 10) {
			*ptr++ = i + '0';
		} else {
			*ptr++ = i + 'a' - 10;
		}
	}

	int len = ptr - tmp;

	while(ptr > tmp) { *str++ = *--ptr; }
	return len;
}
