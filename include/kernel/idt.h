#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct IDTEntry {
	uint16_t base_address_low;
	uint16_t selector;
	uint8_t reserved;
	uint8_t type : 4;
	uint8_t storage_segment : 1;
	uint8_t privilege : 2;
	uint8_t present : 1;
	uint16_t base_address_high;
} __attribute__((packed));

struct IDTDescriptor {
	uint16_t limiter;
	uint32_t base_address;
} __attribute__((packed));

struct IDT {
	struct IDTDescriptor descriptor;
	struct IDTEntry entries[129];
} __attribute__((packed));

void idt_init(struct IDT *);

#endif
