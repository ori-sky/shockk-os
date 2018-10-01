#include <stdint.h>
#include <arch/x86/a20.h>
#include <kernel/ata.h>
#include <kernel/elf.h>
#include <kernel/ext2.h>
#include <kernel/mbr.h>
#include <kernel/pager.h>
#include <kernel/itoa.h>
#include <kernel/panic.h>
#include <kernel/screen.h>
#include <kernel/state.h>

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
		screen << "ident     = " << header.ident     << '\n';
		screen << "type      = " << header.type      << '\n';
		screen << "machine   = " << header.machine   << '\n';
		screen << "version   = " << header.version   << '\n';
		screen << "entry_ptr = " << header.entry_ptr << '\n';
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

enum class BootType {
	Unknown,
	Multiboot,
	SHKBoot
};

extern "C"
void loader_entry(uint32_t mb_magic, uint32_t mb_addr) __attribute__((noreturn));
void loader_entry(uint32_t mb_magic, uint32_t mb_addr) {
	Screen screen;
	screen << "SHK\n";

	BootType boot_type = BootType::Unknown;

	switch(mb_magic) {
	default:
		kernel_panic("invalid boot signature (should be 0x2BADB002 or 0xBAADC0DE)");
	case 0x2BADB002:
		boot_type = BootType::Multiboot;
		break;
	case 0xBAADC0DE:
		a20_enable();
		boot_type = BootType::SHKBoot;
		break;
	}

	struct Pager *pager = Pager::Create();
	pager->Enable();

	ata_init();

	uint32_t starting_lba = 0;
	switch(boot_type) {
	case BootType::Unknown:
		break;
	case BootType::Multiboot: {
		uint8_t *mb = (uint8_t *)mb_addr;
		uint8_t part_id = mb[14];

		MBR mbr = mbr_read();
		starting_lba = mbr.entries[part_id].starting_lba;
		break;
	}
	case BootType::SHKBoot:
		starting_lba = *(uint32_t *)0x7dfa;
		break;
	}

	screen << "starting LBA = " << starting_lba << '\n';

	Ext2 fs(pager, starting_lba);

	const char *paths[] = {"boot", "kernel.elf"};
	auto mKernel = fs.GetInode(2, paths);
	if(mKernel.IsNothing()) { kernel_panic("failed to get /boot/kernel.elf inode"); }
	auto kernel = mKernel.FromJust();

	ELFHeader header;
	fs.ReadInode(kernel, 0, &header);
	screen << header;

	for(size_t p = 0; p < header.ph_count; ++p) {
		uint32_t offset = header.ph_offset + p * header.ph_size;
		ELFProgramHeader ph;
		fs.ReadInode(kernel, offset, &ph);
		screen << ph;

		for(uint32_t addr = ph.v_addr; addr < ph.v_addr + ph.mem_size;
		                               addr += PAGE_ALLOCATOR_PAGE_SIZE) {
			Pager::TableID table = addr / PAGE_ALLOCATOR_PAGE_SIZE / 1024;
			Pager::PageID  page  = addr / PAGE_ALLOCATOR_PAGE_SIZE % 1024;
			if(!pager->GetContext().IsPresent(table, page)) {
				pager->GetContext().AllocAt(table, page);
			}
		}

		char *addr = (char *)ph.v_addr;
		fs.ReadInode(kernel, ph.offset, ph.file_size, addr);

		// zero-initialize rest of memory image
		for(uint32_t byte = ph.file_size; byte < ph.mem_size; ++byte) {
			addr[byte] = 0;
		}
	}

	State state{starting_lba, pager};

	auto kernel_entry = (void(*)(State))header.entry_ptr;
	kernel_entry(state);

	for(;;) { __asm__ ("hlt"); } // unreachable
}
