#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct IDTEntry {
	uint16_t base_address_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t attributes;
	uint16_t base_address_high;
} __attribute__((packed));

struct IDTDescriptor {
	uint16_t limiter;
	uint32_t base_address;
} __attribute__((packed));

struct IDT {
	struct IDTDescriptor descriptor;
	struct IDTEntry entries[256];
} __attribute__((packed));

void idt_init(volatile struct IDT *);

#endif