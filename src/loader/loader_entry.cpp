#include <stdint.h>
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

extern "C" void loader_entry(void) __attribute__((noreturn));
void loader_entry(void) {
	a20_enable();

	struct Pager *pager = Pager::Create();
	pager->Reload();
	pager->Enable();

	ata_init();

	ELFHeader header;
	ata_pio_read(17, 1, &header);

	for(size_t p = 0; p < header.ph_count; ++p) {
		uint32_t offset = header.ph_offset + p * header.ph_size;
		uint8_t ph_sector[1024];
		ata_pio_read(17 + offset / 512, 2, ph_sector);
		ELFProgramHeader *ph = reinterpret_cast<ELFProgramHeader *>(
			&ph_sector[offset % 512]
		);

		for(uint32_t addr = ph->v_addr; addr < ph->v_addr + ph->mem_size;
		                                addr += PAGE_ALLOCATOR_PAGE_SIZE) {
			pager->AllocAt(addr / 1024 / PAGE_ALLOCATOR_PAGE_SIZE,
			               addr / 1024 % PAGE_ALLOCATOR_PAGE_SIZE);
		}

		void *ptr = (void *)ph->v_addr;
		ata_pio_read(17 + ph->offset / 512, ph->file_size / 512 + 1, ptr);
	}

	auto kernel_entry = (void(*)(void))header.entry_ptr;
	kernel_entry();

	for(;;) { __asm__ ("hlt"); }
}
