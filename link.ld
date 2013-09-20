ENTRY(start)
SECTIONS
{
	.text :
	{
		code = .; _code = .; __code = .;
		*(.text)
		. = ALIGN(512);
	}

	.data :
	{
		data = .; _data = .; __data = .;
		*(.data)
		*(.rodata)
		. = ALIGN(512);
	}

	.bss :
	{
		bss = .; _bss = .; _bss = .;
		*(.bss)
		. = ALIGN(512);
	}

	end = .; _end = .; __end = .;
}
