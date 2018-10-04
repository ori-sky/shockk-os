#include <kernel/elf.h>
#include <kernel/ext2.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/itoa.h>
#include <kernel/pager.h>
#include <kernel/panic.h>
#include <kernel/pic.h>
#include <kernel/pit.h>
#include <kernel/ports.h>
#include <kernel/screen.h>
#include <kernel/state.h>
#include <kernel/syscall.h>
#include <kernel/task.h>
#include <kernel/tss.h>
#include <kernel/user.h>

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

extern "C" void kernel_entry(const State) __attribute__((noreturn));
void kernel_entry(const State state) {
	_kernel_state = state;

	Screen screen;
	screen << "SHK\n";

	pit_set(1 << 15); // programmable timer
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);

	IDT *idt = (IDT *)_kernel_state.pager->GetContext().Reserve();
	idt_init(idt);

	__asm__ ("sti");

	GDT *gdt = (GDT *)_kernel_state.pager->GetContext().Reserve();
	TSS *tss = (TSS *)_kernel_state.pager->GetContext().Reserve();
	gdt_init(gdt, tss);

	_kernel_state.tss = tss;

	auto taskOne  = Task::Create("one.elf");
	auto taskTwo  = Task::Create("two.elf");
	auto taskDash = Task::Create("dash.elf");
	auto taskLoop = Task::Create("loop.elf");

	taskOne->next  = taskTwo;
	taskTwo->next  = taskLoop;
	taskLoop->next = taskOne;

	tss_init(tss, taskOne->kernel_stack);
	_kernel_state.task = taskOne;

	__asm__ ("cli");
	task_switch(tss, nullptr, taskOne);

	for(;;) { __asm__ ("hlt"); } // unreachable
}
