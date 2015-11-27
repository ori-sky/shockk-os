#include <stdint.h>
#include <kernel/a20.h>
#include <kernel/pager.h>
#include <kernel/pit.h>
#include <kernel/pic.h>
#include <kernel/irq.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/tss.h>
#include <kernel/panic.h>
#include <kernel/memory.h>
#include <kernel/screen.h>
#include <kernel/syscall.h>
#include <kernel/pci.h>

extern void user_enter(void *) __attribute__((noreturn));

void kernel_main(void) __attribute__((noreturn));
void kernel_main(void) {
	screen_init();

	if(!a20_enable()) { kernel_panic("failed to enable A20 line"); }

	struct Pager *pager = kmalloc(sizeof(struct Pager));
	pager_init(pager);
	pager_test(pager);

	pit_set(1 << 15);
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);

	struct IDT *idt = kmalloc(sizeof(struct IDT));
	idt_init(idt);

	__asm__ ("sti");

	struct PCIEnumeration *pci_enum = kmalloc(sizeof(struct PCIEnumeration));
	pci_enum->count = 0;
	pci_enumerate_buses(pci_enum);
	for(uint16_t i = 0; i < pci_enum->count; ++i) {
		switch(pci_enum->identifiers[i].baseclass) {
		case 0x1: /* mass storage controller */
			switch(pci_enum->identifiers[i].subclass) {
			case 0x1: /* IDE */
				screen_print("found IDE controller\n");
				break;
			case 0x6: /* serial ATA */
				screen_print("found serial ATA controller\n");
				break;
			}
			break;
		}
	}

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
