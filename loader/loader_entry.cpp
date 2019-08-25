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
#include <kernel/screen/elf.h>
#include <kernel/state.h>

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

	State state{screen, fs, pager, nullptr, nullptr, 1, {}};

	auto kernel_entry = (void(*)(State))header.entry_ptr;
	kernel_entry(state);

	for(;;) { __asm__ ("hlt"); } // unreachable
}
