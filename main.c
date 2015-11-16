void entry(void)
{
	unsigned char *ptr = (unsigned char *)0xB8000;
	for(unsigned short col = 0; col < 80*24; ++col) {
		ptr[col << 1] = 'A' + col % 80 % 26;
	}
	for(;;);
}
