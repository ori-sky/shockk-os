#include <itoa.h>

char * uitoa(unsigned int value, char *str, int base)
{
	unsigned char log = 0;
	for(unsigned int n=value; n>=base; n/=base) ++log;

	str[log+1] = 0;

	unsigned short div = 1;
	for(unsigned char i=log; i!=(unsigned char)(-1); --i, div*=base)
	{
		str[i] = '0' + value / div % base;
	}

	return str;
}
