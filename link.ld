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
		. = ALIGN(512);
	}

	.rodata :
	{
		rodata = .; _rodata = .; __rodata = .;
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
