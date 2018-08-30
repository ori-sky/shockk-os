ENTRY(_start)
SECTIONS {
	. = 32K;

	.text BLOCK(4K) : ALIGN(4K) {
		*(.multiboot)
		*(.text)
	}
	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata)
	}
	.data BLOCK(4K) : ALIGN(4K) {
		*(.data)
	}
	.bss BLOCK(4K) : ALIGN(4K) {
		*(COMMON)
		*(.bss)
	}
}
