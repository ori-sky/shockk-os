#include <stdint.h>
#include <kernel/itoa.h>
#include <kernel/screen.h>
#include <kernel/ata.h>
#include <arch/x86/a20.h>

enum class ELFVersion : uint8_t {
	None = 0,
	ELF1 = 1
};

enum class ELFClass : uint8_t {
	None = 0,
	x32  = 1,
	x64  = 2
};

enum class ELFEncoding : uint8_t {
	None         = 0,
	LittleEndian = 1,
	BigEndian    = 2
};

enum class ELFABI : uint8_t {
	SystemV = 0x00,
	Linux   = 0x03,
	SHK     = 0x7F
};

struct ELFIdent {
	uint8_t     magic[4];
	ELFClass    class_;
	ELFEncoding encoding;
	ELFVersion  version;
	ELFABI      abi;
	uint8_t     abi_version;
	uint8_t     padding[6];
} __attribute__((packed));

enum class ELFType : uint16_t {
	None        = 0,
	Relocatable = 1,
	Executable  = 2,
	Dynamic     = 3,
	Core        = 4,
	LowProc     = 0xFF00,
	HighProc    = 0xFFFF
};

enum class ELFMachine : uint16_t {
	None     = 0,
	Intel386 = 3
};

struct ELFHeader {
	ELFIdent   ident;
	ELFType    type;
	ELFMachine machine;
	ELFVersion version;
	uint32_t   entry_ptr;
	uint32_t   program_header_off;
	uint32_t   section_header_off;
	uint32_t   flags;
	uint16_t   elf_header_size;
	uint16_t   program_header_entry_size;
	uint16_t   program_header_count;
	uint16_t   section_header_entry_size;
	uint16_t   section_header_count;
	uint16_t   section_header_str_idx;
} __attribute__((packed));

struct ELF {
	ELFHeader header;
} __attribute__((packed));

extern "C" void loader_entry(void) __attribute__((noreturn));
void loader_entry(void) {
	a20_enable();
	ata_init();
	ELF *elf = (ELF *)0x10000;
	ata_pio_read(17, 1, elf);

	char v[16];
	itoa(static_cast<int>(elf->header.version), v, 16);
	screen_init();
	screen_put(elf->header.ident.magic[1]);
	screen_put(elf->header.ident.magic[2]);
	screen_put(elf->header.ident.magic[3]);
	screen_put('\n');
	screen_print("0x");
	screen_print(v);

	for(;;) { __asm__ ("hlt"); }
}
