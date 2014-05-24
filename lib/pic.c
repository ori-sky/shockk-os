#include <pic.h>
#include <ports.h>

void pic_set_masks(unsigned char mask_master, unsigned char mask_slave)
{
	ports_out(PIC_PORT_MASTER_DATA, mask_master);
	ports_out(PIC_PORT_SLAVE_DATA, mask_slave);
}

void pic_remap(unsigned char offset_master, unsigned char offset_slave)
{
	/* save data masks */
	unsigned char mask_master = ports_in(PIC_PORT_MASTER_DATA);
	unsigned char mask_slave = ports_in(PIC_PORT_SLAVE_DATA);

	/* ICW1 */
	ports_out(PIC_PORT_MASTER_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	ports_out(PIC_PORT_SLAVE_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);

	/* ICW2 */
	ports_out(PIC_PORT_MASTER_DATA, offset_master);
	ports_out(PIC_PORT_SLAVE_DATA, offset_slave);

	/* ICW3
	 * master - slave PIC at IRQ2 (bit 2)
	 * slave  - attached to IRQ2 (2)
	 */
	ports_out(PIC_PORT_MASTER_DATA, 0b00000100);
	ports_out(PIC_PORT_SLAVE_DATA, 2);

	/* ICW4 */
	ports_out(PIC_PORT_MASTER_DATA, PIC_ICW4_8086);
	ports_out(PIC_PORT_SLAVE_DATA, PIC_ICW4_8086);

	/* restore data masks */
	ports_out(PIC_PORT_MASTER_DATA, mask_master);
	ports_out(PIC_PORT_SLAVE_DATA, mask_slave);
}
