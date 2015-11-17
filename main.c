#include <stdint.h>

extern void isrStub(void);
extern void isrErrorCodeStub(void);

struct IDTEntry {
	uint16_t baseAddressLow;
	uint16_t selector;
	uint8_t zero;
	uint8_t attributes;
	uint16_t baseAddressHigh;
} __attribute__((packed));

struct IDTDesc {
	uint16_t limiter;
	uint32_t baseAddress;
} __attribute__((packed));

inline void setEntry(struct IDTEntry *entry, void (*handler)(void), uint8_t attributes) {
	entry->baseAddressLow = handler;
	entry->selector = 0x8;
	entry->zero = 0;
	entry->attributes = attributes;
	entry->baseAddressHigh = 0;
}

void entry(void) {
	const unsigned short numEntries = 256;

	struct IDTEntry *idt = (struct IDTEntry *)0x800;
	setEntry(&idt[ 0], isrStub, 0b10001110);
	setEntry(&idt[ 1], isrStub, 0b10001110);
	setEntry(&idt[ 2], isrStub, 0b10001110);
	setEntry(&idt[ 3], isrStub, 0b10001110);
	setEntry(&idt[ 4], isrStub, 0b10001110);
	setEntry(&idt[ 5], isrStub, 0b10001110);
	setEntry(&idt[ 6], isrStub, 0b10001110);
	setEntry(&idt[ 7], isrStub, 0b10001110);
	setEntry(&idt[ 8], isrErrorCodeStub, 0b10001110);
	setEntry(&idt[ 9], isrStub, 0b10001110);
	setEntry(&idt[10], isrErrorCodeStub, 0b10001110);
	setEntry(&idt[11], isrErrorCodeStub, 0b10001110);
	setEntry(&idt[12], isrErrorCodeStub, 0b10001110);
	setEntry(&idt[13], isrErrorCodeStub, 0b10001110);
	setEntry(&idt[14], isrErrorCodeStub, 0b10001110);
	for(unsigned short entry = 15; entry < numEntries; ++entry) {
		setEntry(&idt[entry], isrStub, 0b10001110);
	}

	struct IDTDesc *idtDesc = (struct IDTDesc *)0x7FA;
	idtDesc->limiter = 0x800;
	idtDesc->baseAddress = 0x800;

	__asm__ __volatile__ ("lidt 0x7FA" ::: "memory");
	__asm__ __volatile__ ("sti");

	unsigned char *ptr = (unsigned char *)0xB8000;
	for(unsigned short col = 0; col < 80 * 25; ++col) {
		ptr[col << 1] = 'A' + col % 80 % 26;
	}

	for(;;) {
		__asm__ __volatile__ ("hlt");
	}
}

void isrHandler(void) {
	static unsigned int i = 0;
	unsigned char *ptr = (unsigned char *)0xB8000;
	ptr[0] = 'a' + i % 26;
	++i;
}
