ENTRY(entry)
SECTIONS
{
	.text :
	{
		*(.text)
		. = ALIGN(512);
	}

	.data :
	{
		*(.data)
		. = ALIGN(512);
	}

	.rodata :
	{
		*(.rodata)
		. = ALIGN(512);
	}

	.bss :
	{
		*(.bss)
		. = ALIGN(512);
	}
}
