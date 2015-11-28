#include <stdbool.h>
#include <kernel/idt.h>

extern void isr_stub_0(void);
extern void isr_stub_1(void);
extern void isr_stub_32(void);
extern void isr_stub_128(void);

void idt_set_entry(struct IDTEntry *entry, void (*handler)(void), bool restricted) {
	uint32_t base_address = (uint32_t)handler;
	entry->base_address_low = base_address & 0xFFFF;
	entry->base_address_high = (base_address >> 16) & 0xFFFF;
	entry->selector = 0x8;
	entry->reserved = 0;
	entry->type = 0xE; /* 80386 32-bit interrupt gate */
	entry->storage_segment = 0; /* 0 for interrupt gates */
	entry->privilege = restricted ? 0 : 3;
	entry->present = 1;
}

void idt_init(struct IDT *idt) {
	idt->descriptor.limiter = sizeof(idt->entries);
	idt->descriptor.base_address = (uint32_t)&idt->entries;

	uint32_t stub_size = (uint32_t)isr_stub_1 - (uint32_t)isr_stub_0;

	uint16_t entry;
	for(entry = 0; entry < 128; ++entry) {
		idt->entries[entry].present = 0;
	}
	for(entry = 0; entry < 20; ++entry) {
		void (*stub)(void) = (void (*)(void))((uint32_t)isr_stub_0 + entry * stub_size);
		idt_set_entry(&idt->entries[entry], stub, true);
	}
	for(entry = 32; entry < 48; ++entry) {
		void (*stub)(void) = (void (*)(void))((uint32_t)isr_stub_32 + (entry - 32) * stub_size);
		idt_set_entry(&idt->entries[entry], stub, true);
	}
	idt_set_entry(&idt->entries[128], isr_stub_128, false);

	__asm__ ("lidt (%0)" : : "r" (&idt->descriptor));
}
