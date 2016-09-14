#include <arch/x86/a20.h>
#include <kernel/panic.h>
#include <kernel/pager.h>
#include <kernel/ata.h>

void loader_entry(void) __attribute__((noreturn));
void loader_entry(void) {
	a20_enable();

	struct Pager *pager = pager_init();
	pager_reload(pager);
	pager_enable();

	ata_init();

	volatile uint8_t *kernel = pager_reserve(pager);
	if(kernel == NULL) { kernel_panic("failed to reserve page for kernel"); }

	/* read infosector */
	ata_pio_read(1, 1, kernel);
	uint8_t loader_sectors = kernel[0];
	uint8_t kernel_sectors = kernel[1];
	size_t kernel_bytes = kernel_sectors * 512u;
	size_t kernel_pages = (kernel_bytes + PAGE_ALLOCATOR_PAGE_SIZE - 1) / PAGE_ALLOCATOR_PAGE_SIZE;

	/* reserve remaining pages required for kernel */
	for(uint32_t i = 1; i < kernel_pages; ++i) {
		if(pager_reserve(pager) == NULL) { kernel_panic("failed to reserve page for kernel"); }
	}

	/* read kernel starting from bootsector + infosector + loader sector count */
	ata_pio_read(2u + loader_sectors, kernel_sectors, kernel);

	/* call kernel entry point */
	void (*kernel_entry)(struct Pager *) = (void (*)(struct Pager *))(uintptr_t)kernel;
	kernel_entry(pager);

	for(;;) { __asm__ ("hlt"); }
}
