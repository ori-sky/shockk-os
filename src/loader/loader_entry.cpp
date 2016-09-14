#include <stdint.h>
#include <kernel/itoa.h>
#include <kernel/screen.h>
#include <kernel/ata.h>
#include <kernel/pager.h>
#include <arch/x86/a20.h>

enum class ELFIdentVersion : uint8_t {
	None = 0,
	ELF1 = 1
};

enum class ELFIdentClass : uint8_t {
	None = 0,
	x32  = 1,
	x64  = 2
};

enum class ELFIdentEncoding : uint8_t {
	None         = 0,
	LittleEndian = 1,
	BigEndian    = 2
};

enum class ELFIdentABI : uint8_t {
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
};

enum class ELFMachine : uint16_t {
	None     = 0,
	Intel386 = 3
};

struct ELFIdent {
	uint8_t          magic[4];
	ELFIdentClass    class_;
	ELFIdentEncoding encoding;
	ELFIdentVersion  version;
	ELFIdentABI      abi;
	uint8_t          abi_version;
	uint8_t          padding[7];
} __attribute__((packed));

struct ELFHeader {
	ELFIdent   ident;
	ELFType    type;
	ELFMachine machine;
	uint32_t   version;   // we can just use ELFIdent.version which is a uint8_t
	uint32_t   entry_ptr;
	uint32_t   ph_offset; // program header offset
	uint32_t   sh_offset; // section header offset
	uint32_t   flags;
	uint16_t   elf_header_size;
	uint16_t   ph_size;
	uint16_t   ph_count;
	uint16_t   sh_size;
	uint16_t   sh_count;
	uint16_t   sh_str_idx;
} __attribute__((packed));

enum class ELFProgramType : uint32_t {
	Null    = 0,
	Load    = 1,
	Dynamic = 2,
	Interp  = 3,
	Note    = 4,
	Shared  = 5,
	Header  = 6
};

struct ELFProgramHeader {
	ELFProgramType type;
	uint32_t offset;
	uint32_t v_addr;
	uint32_t p_addr;
	uint32_t file_size;
	uint32_t mem_size;
	uint32_t flags;
	uint32_t align;
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

	friend Screen & operator<<(Screen &screen, const ELFIdentClass class_) {
		switch(class_) {
			SCREEN_CASE   (ELFIdentClass::None);
			SCREEN_CASE   (ELFIdentClass::x32);
			SCREEN_CASE   (ELFIdentClass::x64);
			SCREEN_DEFAULT(ELFIdentClass, class_);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFIdentEncoding encoding) {
		switch(encoding) {
			SCREEN_CASE   (ELFIdentEncoding::None);
			SCREEN_CASE   (ELFIdentEncoding::LittleEndian);
			SCREEN_CASE   (ELFIdentEncoding::BigEndian);
			SCREEN_DEFAULT(ELFIdentEncoding, encoding);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFIdentVersion version) {
		switch(version) {
			SCREEN_CASE   (ELFIdentVersion::None);
			SCREEN_CASE   (ELFIdentVersion::ELF1);
			SCREEN_DEFAULT(ELFIdentVersion, version);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFIdentABI abi) {
		switch(abi) {
			SCREEN_CASE   (ELFIdentABI::SystemV);
			SCREEN_CASE   (ELFIdentABI::Linux);
			SCREEN_CASE   (ELFIdentABI::SHK);
			SCREEN_DEFAULT(ELFIdentABI, abi);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFType type) {
		switch(type) {
			SCREEN_CASE   (ELFType::None);
			SCREEN_CASE   (ELFType::Relocatable);
			SCREEN_CASE   (ELFType::Executable);
			SCREEN_CASE   (ELFType::Dynamic);
			SCREEN_CASE   (ELFType::Core);
			SCREEN_DEFAULT(ELFType, type);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFMachine machine) {
		switch(machine) {
			SCREEN_CASE   (ELFMachine::None);
			SCREEN_CASE   (ELFMachine::Intel386);
			SCREEN_DEFAULT(ELFMachine, machine);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFIdent &ident) {
		screen << "ELFIdent::magic       = " << static_cast<char>(ident.magic[0])
		                                     << static_cast<char>(ident.magic[1])
		                                     << static_cast<char>(ident.magic[2])
		                                     << static_cast<char>(ident.magic[3])
		                                                          << '\n';
		screen << "ELFIdent::class       = " << ident.class_      << '\n';
		screen << "ELFIdent::encoding    = " << ident.encoding    << '\n';
		screen << "ELFIdent::version     = " << ident.version     << '\n';
		screen << "ELFIdent::abi         = " << ident.abi         << '\n';
		screen << "ELFIdent::abi_version = " << ident.abi_version << '\n';
		return screen;
	}

	friend Screen & operator<<(Screen &screen, const ELFHeader &header) {
		screen << header.ident;
		screen << "ELFHeader::type       = " << header.type       << '\n';
		screen << "ELFHeader::machine    = " << header.machine    << '\n';
		//screen << "ELFHeader::version    = " << header.version    << '\n';
		screen << "ELFHeader::entry_ptr  = " << header.entry_ptr  << '\n';
		screen << "ELFHeader::ph_offset  = " << header.ph_offset  << '\n';
		screen << "ELFHeader::sh_offset  = " << header.sh_offset  << '\n';
		screen << "ELFHeader::flags      = " << header.flags      << '\n';
		screen << "ELFHeader::ph_size    = " << header.ph_size    << '\n';
		screen << "ELFHeader::ph_count   = " << header.ph_count   << '\n';
		screen << "ELFHeader::sh_size    = " << header.sh_size    << '\n';
		screen << "ELFHeader::sh_count   = " << header.sh_count   << '\n';
		screen << "ELFHeader::sh_str_idx = " << header.sh_str_idx << '\n';
		return screen;
	}

	friend Screen & operator<<(Screen &screen, const ELFProgramType type) {
		switch(type) {
			SCREEN_CASE   (ELFProgramType::Null);
			SCREEN_CASE   (ELFProgramType::Load);
			SCREEN_CASE   (ELFProgramType::Dynamic);
			SCREEN_CASE   (ELFProgramType::Interp);
			SCREEN_CASE   (ELFProgramType::Note);
			SCREEN_CASE   (ELFProgramType::Shared);
			SCREEN_CASE   (ELFProgramType::Header);
			SCREEN_DEFAULT(ELFProgramType, type);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFProgramHeader &ph) {
		screen << "ELFProgramHeader::type      = " << ph.type      << '\n';
		screen << "ELFProgramHeader::offset    = " << ph.offset    << '\n';
		screen << "ELFProgramHeader::v_addr    = " << ph.v_addr    << '\n';
		screen << "ELFProgramHeader::p_addr    = " << ph.p_addr    << '\n';
		screen << "ELFProgramHeader::file_size = " << ph.file_size << '\n';
		screen << "ELFProgramHeader::mem_size  = " << ph.mem_size  << '\n';
		screen << "ELFProgramHeader::flags     = " << ph.flags     << '\n';
		screen << "ELFProgramHeader::align     = " << ph.align     << '\n';
		return screen;
	}
};

extern "C" void loader_entry(void) __attribute__((noreturn));
void loader_entry(void) {
	a20_enable();

	struct Pager *pager = pager_init();
	pager_reload(pager);
	pager_enable();

	ata_init();

	ELFHeader header;
	ata_pio_read(17, 1, &header);

	uint8_t ph_sector[512 * 2];
	ata_pio_read(17 + header.ph_offset / 512, 2, ph_sector);
	ELFProgramHeader *ph = reinterpret_cast<ELFProgramHeader *>(
		&ph_sector[header.ph_offset % 512]
	);

	void *entry_ptr = pager_reserve(pager);
	for(unsigned int i = 1; i < ph->mem_size / PAGE_ALLOCATOR_PAGE_SIZE + 1; ++i) {
		pager_reserve(pager);
	}

	ata_pio_read(17 + ph->offset / 512, ph->mem_size / 512 + 1, entry_ptr);

	auto kernel_entry = reinterpret_cast<void(*)(void)>(entry_ptr);
	kernel_entry();

	for(;;) { __asm__ ("hlt"); }
}
