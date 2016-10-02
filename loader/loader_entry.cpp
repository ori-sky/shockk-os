#include <stdint.h>
#include <kernel/ext2.h>
#include <kernel/ata.h>
#include <kernel/mbr.h>
#include <kernel/pager.h>
#include <kernel/itoa.h>
#include <kernel/panic.h>
#include <kernel/screen.h>

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
			SCREEN_CASE(ELFIdentClass::None);
			SCREEN_CASE(ELFIdentClass::x32);
			SCREEN_CASE(ELFIdentClass::x64);
			SCREEN_DEFAULT(ELFIdentClass, class_);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFIdentEncoding encoding) {
		switch(encoding) {
			SCREEN_CASE(ELFIdentEncoding::None);
			SCREEN_CASE(ELFIdentEncoding::LittleEndian);
			SCREEN_CASE(ELFIdentEncoding::BigEndian);
			SCREEN_DEFAULT(ELFIdentEncoding, encoding);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFIdentVersion version) {
		switch(version) {
			SCREEN_CASE(ELFIdentVersion::None);
			SCREEN_CASE(ELFIdentVersion::ELF1);
			SCREEN_DEFAULT(ELFIdentVersion, version);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFIdentABI abi) {
		switch(abi) {
			SCREEN_CASE(ELFIdentABI::SystemV);
			SCREEN_CASE(ELFIdentABI::Linux);
			SCREEN_CASE(ELFIdentABI::SHK);
			SCREEN_DEFAULT(ELFIdentABI, abi);
		}
	}

	friend Screen & operator<<(Screen &screen, const ELFType type) {
		switch(type) {
			SCREEN_CASE(ELFType::None);
			SCREEN_CASE(ELFType::Relocatable);
			SCREEN_CASE(ELFType::Executable);
			SCREEN_CASE(ELFType::Dynamic);
			SCREEN_CASE(ELFType::Core);
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

	friend Screen & operator<<(Screen &screen, const ELFProgramHeader &ph) {
		screen << "offset    = " << ph.offset    << '\n';
		screen << "v_addr    = " << ph.v_addr    << '\n';
		screen << "p_addr    = " << ph.p_addr    << '\n';
		screen << "file_size = " << ph.file_size << '\n';
		screen << "mem_size  = " << ph.mem_size  << '\n';
		screen << "flags     = " << ph.flags     << '\n';
		screen << "align     = " << ph.align     << '\n';
		return screen;
	}
};

extern "C"
void loader_entry(uint32_t mb_magic, uint32_t mb_addr) __attribute__((noreturn));
void loader_entry(uint32_t mb_magic, uint32_t mb_addr) {
	if(mb_magic != 0x2BADB002) {
		kernel_panic("invalid multiboot signature (should be 0x2BADB002)");
	}

	uint8_t *mb = (uint8_t *)mb_addr;
	uint8_t part_id = mb[14];

	struct Pager *pager = Pager::Create();
	pager->Reload();
	pager->Enable();

	ata_init();
	MBR mbr = mbr_read();
	Ext2 fs(pager, mbr.entries[part_id].starting_lba);

	const char *paths[] = {"boot", "kernel"};
	auto mKernel = fs.GetInode(2, paths);
	if(mKernel.IsNothing()) { kernel_panic("failed to get /boot/kernel inode"); }
	auto kernel = mKernel.FromJust();

	ELFHeader header;
	fs.ReadInode(kernel, 0, &header);

	Screen screen;

	for(size_t p = 0; p < header.ph_count; ++p) {
		uint32_t offset = header.ph_offset + p * header.ph_size;
		ELFProgramHeader ph;
		fs.ReadInode(kernel, offset, &ph);
		//screen << ph;

		for(uint32_t addr = ph.v_addr; addr < ph.v_addr + ph.mem_size;
		                               addr += PAGE_ALLOCATOR_PAGE_SIZE) {
			Pager::TableID table = addr / PAGE_ALLOCATOR_PAGE_SIZE / 1024;
			Pager::PageID  page  = addr / PAGE_ALLOCATOR_PAGE_SIZE % 1024;
			if(!pager->IsPresent(table, page)) {
				pager->AllocAt(table, page);
			}
		}

		char *addr = (char *)ph.v_addr;
		fs.ReadInode(kernel, ph.offset, ph.file_size, addr);

		// zero-initialize rest of memory image
		for(uint32_t byte = ph.file_size; byte < ph.mem_size; ++byte) {
			addr[byte] = 0;
		}
	}

	auto kernel_entry = (void(*)(Pager *))header.entry_ptr;
	//screen << (uint32_t)kernel_entry;
	kernel_entry(pager);

	for(;;) { __asm__ ("hlt"); } // unreachable
}
