ENTRY(entry)
SECTIONS
{
	.text BLOCK(512) : ALIGN(512)
	{
		*(.text)
	}

	.rodata BLOCK(512) : ALIGN(512)
	{
		*(.rodata)
	}

	.data BLOCK(512) : ALIGN(512)
	{
		*(.data)
	}

	.bss BLOCK(512) : ALIGN(512)
	{
		*(COMMON)
		*(.bss)
	}
}
