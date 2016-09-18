#include <kernel/panic.h>
#include <kernel/pager.h>
#include <kernel/itoa.h>
#include <kernel/pit.h>
#include <kernel/pic.h>
#include <kernel/irq.h>
#include <kernel/screen.h>

extern "C" void kernel_entry(Pager *) __attribute__((noreturn));
void kernel_entry(Pager *pager) {
	screen_init();
	pit_set(1 << 15);
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);

	kernel_panic("call to kernel_entry succeeded!");
	for(;;) { __asm__ ("hlt"); }
}
