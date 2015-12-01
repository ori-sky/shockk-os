#include <stdint.h>
#include <kernel/a20.h>
#include <kernel/memory.h>
#include <kernel/page_allocator.h>
#include <kernel/pager.h>
#include <kernel/pit.h>
#include <kernel/pic.h>
#include <kernel/irq.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/tss.h>
#include <kernel/panic.h>
#include <kernel/screen.h>
#include <kernel/syscall.h>
#include <kernel/pci.h>
#include <kernel/ata.h>

extern void user_enter(void *) __attribute__((noreturn));

void kernel_main(void) __attribute__((noreturn));
void kernel_main(void) {
	screen_init();

	if(!a20_enable()) { kernel_panic("failed to enable A20 line"); }

	struct PageAllocator *page_allocator = kmalloc(sizeof(struct PageAllocator));
	page_allocator_init(page_allocator);
	page_allocator_test(page_allocator);

	struct Pager *pager = kmalloc(sizeof(struct Pager));
	pager_init(pager, page_allocator);
	pager_enable();

	pit_set(1 << 15);
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);

	struct IDT *idt = kmalloc(sizeof(struct IDT));
	idt_init(idt);

	__asm__ ("sti");

	struct PCIEnumeration *pci_enum = kmalloc(sizeof(struct PCIEnumeration));
	pci_enumerate_buses(pci_enum);
	for(uint16_t i = 0; i < pci_enum->count; ++i) {
		switch(pci_enum->info[i].baseclass) {
		case 0x1: /* mass storage controller */
			switch(pci_enum->info[i].subclass) {
			case 0x1: /* ATA */
				screen_print("found ATA controller\n");
				break;
			case 0x6: /* serial ATA */
				screen_print("found serial ATA controller\n");
				break;
			}
			break;
		}
	}

	ata_init();
	screen_print("ATA driver loading bootsector\n");

	volatile uint16_t *sector_ptr = kmalloc(512);
	ata_pio_read(0, 2, sector_ptr);

	if(sector_ptr[255] != 0xAA55) { kernel_panic("ATA driver failed to load bootsector"); }
	else { screen_print("bootsector signature verified\n"); }

	if(sector_ptr[256] != *(uint16_t *)0x10000) { kernel_panic("ATA driver failed to load kernel"); }
	else { screen_print("kernel sector verified\n"); }

	struct GDT *gdt = kmalloc(sizeof(struct GDT));
	struct TSS *tss = kmalloc(sizeof(struct TSS));
	gdt_init(gdt, tss);
	tss_init(tss);

	unsigned char *user_stack = kmalloc(1 << 20);
	user_enter(&user_stack[1 << 20]);
}

void user_main(void) __attribute__((noreturn));
void user_main(void) {
	syscall_put(':');
	syscall_put('D');
	syscall_put('\n');
	syscall_put('\n');
	for(;;);
}
