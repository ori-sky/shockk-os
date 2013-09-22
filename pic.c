#include "pic.h"
#include "ports.h"

void pic_remap(unsigned char master_offset, unsigned char slave_offset)
{
	// save data masks
	unsigned char master_mask = ports_in(PIC_PORT_MASTER + 1);
	unsigned char slave_mask = ports_in(PIC_PORT_SLAVE + 1);

	// ICW1
	ports_out(PIC_PORT_MASTER, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	ports_out(PIC_PORT_SLAVE, PIC_ICW1_INIT | PIC_ICW1_ICW4);

	// ICW2 = vector offset
	ports_out(PIC_PORT_MASTER, master_offset);
	ports_out(PIC_PORT_SLAVE, slave_offset);

	// ICW3 = master/slave wired info
	// 4 = bit 2 = IRQ2 slave
	ports_out(PIC_PORT_MASTER, 4);
	// cascade identity = IRQ2
	ports_out(PIC_PORT_SLAVE, 2);

	// ICW4 = additional environment info
	ports_out(PIC_PORT_MASTER, PIC_ICW4_8086);
	ports_out(PIC_PORT_SLAVE, PIC_ICW4_8086);

	// restore data masks
	ports_out(PIC_PORT_MASTER + 1, master_mask);
	ports_out(PIC_PORT_SLAVE + 1, slave_mask);
}
