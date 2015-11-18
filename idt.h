#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct IDTEntry {
	uint16_t baseAddressLow;
	uint16_t selector;
	uint8_t zero;
	uint8_t attributes;
	uint16_t baseAddressHigh;
} __attribute__((packed));

struct IDTDescriptor {
	uint16_t limiter;
	uint32_t baseAddress;
} __attribute__((packed));

struct IDT {
	struct IDTEntry entries[256];
	struct IDTDescriptor descriptor;
} __attribute__((packed));

void idt_init(volatile struct IDT *);

#endif
