#include <stdint.h>
#include <kernel/a20.h>
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

extern void user_enter(void *);

void kernel_main(void) __attribute__((noreturn));
void kernel_main(void) {
	struct IDT *idt = kmalloc(sizeof(struct IDT));
	struct TSS *tss = kmalloc(sizeof(struct TSS));
	struct GDT *gdt = kmalloc(sizeof(struct GDT));
	struct PCIEnumeration *pci_enum = kmalloc(sizeof(struct PCIEnumeration));
	pci_enum->count = 0;

	screen_init();

	if(!a20_enable()) { kernel_panic("failed to enable A20 line"); }

	pit_set(1 << 15);
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);
	idt_init(idt);

	__asm__ ("sti");

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

	gdt_init(gdt, tss);
	tss_init(tss);

	user_enter(kmalloc(1 << 20) + (1 << 20));
	for(;;) { __asm__ ("hlt"); }
}

void user_main(void) __attribute__((noreturn));
void user_main(void) {
	syscall_put(':');
	syscall_put('D');
	syscall_put('\n');
	syscall_put('\n');
	for(;;);
}
