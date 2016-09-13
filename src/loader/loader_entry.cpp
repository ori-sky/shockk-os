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

struct ELFIdent {
	uint8_t     magic[4];
	ELFClass    class_;
	ELFEncoding encoding;
	ELFVersion  version;
	ELFABI      abi;
	uint8_t     abi_version;
	uint8_t     padding[7];
} __attribute__((packed));

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

#define SCREEN_CASE(X)       case X:  return screen << #X
#define SCREEN_DEFAULT(X, Y) default: return screen << #X << "::<INVALID> (" << static_cast<uint32_t>(Y) << ')'

class Screen {
public:
	Screen(void) {
		screen_init();
	}

	friend Screen & operator<<(Screen &screen, const char c) {
		screen_put(c);
		return screen;
	}

	friend Screen & operator<<(Screen &screen, const uint8_t i) {
		char sz[4] = {0};
		itoa(static_cast<int>(i), sz, 16);
		return screen << "0x" << sz;
	}

	friend Screen & operator<<(Screen &screen, const uint16_t i) {
		char sz[8] = {0};
		itoa(static_cast<int>(i), sz, 16);
		return screen << "0x" << sz;
	}

	friend Screen & operator<<(Screen &screen, const uint32_t i) {
		char sz[16] = {0};
		itoa(static_cast<int>(i), sz, 16);
		return screen << "0x" << sz;
	}

	friend Screen & operator<<(Screen &screen, const char *sz) {
		screen_print(sz);
		return screen;
	}

	friend Screen & operator<<(Screen &screen, const ELFClass class_) {
		switch(class_) {
			SCREEN_CASE(ELFClass::None);
			SCREEN_CASE(ELFClass::x32);
			SCREEN_CASE(ELFClass::x64);
			SCREEN_DEFAULT(ELFClass, class_);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFEncoding encoding) {
		switch(encoding) {
			SCREEN_CASE(ELFEncoding::None);
			SCREEN_CASE(ELFEncoding::LittleEndian);
			SCREEN_CASE(ELFEncoding::BigEndian);
			SCREEN_DEFAULT(ELFEncoding, encoding);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFVersion version) {
		switch(version) {
			SCREEN_CASE(ELFVersion::None);
			SCREEN_CASE(ELFVersion::ELF1);
			SCREEN_DEFAULT(ELFVersion, version);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFABI abi) {
		switch(abi) {
			SCREEN_CASE(ELFABI::SystemV);
			SCREEN_CASE(ELFABI::Linux);
			SCREEN_CASE(ELFABI::SHK);
			SCREEN_DEFAULT(ELFABI, abi);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFType type) {
		switch(type) {
			SCREEN_CASE(ELFType::None);
			SCREEN_CASE(ELFType::Relocatable);
			SCREEN_CASE(ELFType::Executable);
			SCREEN_CASE(ELFType::Dynamic);
			SCREEN_CASE(ELFType::Core);
			SCREEN_CASE(ELFType::LowProc);
			SCREEN_CASE(ELFType::HighProc);
			SCREEN_DEFAULT(ELFType, type);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFMachine machine) {
		switch(machine) {
			SCREEN_CASE(ELFMachine::None);
			SCREEN_CASE(ELFMachine::Intel386);
			SCREEN_DEFAULT(ELFMachine, machine);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFIdent &ident) {
		screen << '\n';
		screen << "  magic       = " << static_cast<char>(ident.magic[0])
		                             << static_cast<char>(ident.magic[1])
		                             << static_cast<char>(ident.magic[2])
		                             << static_cast<char>(ident.magic[3])
		                                                  << '\n';
		screen << "  class       = " << ident.class_      << '\n';
		screen << "  encoding    = " << ident.encoding    << '\n';
		screen << "  version     = " << ident.version     << '\n';
		screen << "  abi         = " << ident.abi         << '\n';
		screen << "  abi_version = " << ident.abi_version;
		return screen;
	}

	friend Screen & operator<<(Screen &screen, const ELFHeader &header) {
		screen << "ident   = " << header.ident   << '\n';
		screen << "type    = " << header.type    << '\n';
		screen << "machine = " << header.machine << '\n';
		screen << "version = " << header.version << '\n';
		return screen;
	}

	friend Screen & operator<<(Screen &screen, const ELF &elf) {
		return screen << elf.header;
	}
};

extern "C" void loader_entry(void) __attribute__((noreturn));
void loader_entry(void) {
	a20_enable();
	ata_init();
	ELF *elf = (ELF *)0x10000;
	ata_pio_read(17, 1, elf);

	Screen screen;
	screen << *elf;

	for(;;) { __asm__ ("hlt"); }
}
