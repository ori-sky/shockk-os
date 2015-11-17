#include "pic.h"
#include "ports.h"

void pic_set_masks(unsigned char mask_master, unsigned char mask_slave) {
	ports_outb(PIC_PORT_MASTER_DATA, mask_master);
	ports_outb(PIC_PORT_SLAVE_DATA, mask_slave);
}

void pic_remap(unsigned char offset_master, unsigned char offset_slave) {
	/* save data masks */
	unsigned char mask_master = ports_inb(PIC_PORT_MASTER_DATA);
	unsigned char mask_slave = ports_inb(PIC_PORT_SLAVE_DATA);

	/* write ICW1 */
	ports_outb(PIC_PORT_MASTER_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	ports_outb(PIC_PORT_SLAVE_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);

	/* write ICW2 (vector offset) */
	ports_outb(PIC_PORT_MASTER_DATA, offset_master);
	ports_outb(PIC_PORT_SLAVE_DATA, offset_slave);

	/* write ICW3 (master/slave wiring info)
	 * slave PIC at IRQ2 (bit 2)
	 * slave's cascade identity is IRQ2
	 */
	ports_outb(PIC_PORT_MASTER_DATA, 0b00000100);
	ports_outb(PIC_PORT_SLAVE_DATA, 2);

	/* write ICW4 (additional environment info) */
	ports_outb(PIC_PORT_MASTER_DATA, PIC_ICW4_8086);
	ports_outb(PIC_PORT_SLAVE_DATA, PIC_ICW4_8086);

	/* restore data masks */
	pic_set_masks(mask_master, mask_slave);
}
