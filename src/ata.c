#include <kernel/ports.h>
#include <kernel/panic.h>
#include <kernel/ata.h>

void ata_init(void) {
	/* check for floating bus (no drives) */
	if(ports_inb(ATA_PORT_PRIMARY_STATUS) == 0xFF) { kernel_panic("ATA: floating bus detected"); }

	ata_soft_reset();
}

void ata_soft_reset(void) {
	ports_outb(ATA_PORT_PRIMARY_CONTROL, ATA_CONTROL_SOFT_RESET);
	while((ports_inb(ATA_PORT_PRIMARY_STATUS) & ATA_STATUS_BUSY) == 0) {}

	/* clear reset bit */
	ports_outb(ATA_PORT_PRIMARY_CONTROL, 0);

	while(ports_inb(ATA_PORT_PRIMARY_STATUS) & ATA_STATUS_BUSY) {}

	uint8_t diag = ports_inb(ATA_PORT_PRIMARY_ERROR);
	if((diag & ATA_DIAGNOSTIC_DEVICE_0_PASS) == 0) { kernel_panic("ATA: device 0 failed diagnostic"); }
	if( diag & ATA_DIAGNOSTIC_DEVICE_1_FAIL)       { kernel_panic("ATA: device 1 failed diagnostic"); }

	while((ports_inb(ATA_PORT_PRIMARY_STATUS) & ATA_STATUS_READY) == 0) {}
}

void ata_pio_begin(void) {
	while(ports_inb(ATA_PORT_PRIMARY_STATUS) & ATA_STATUS_BUSY) {}

	/* write device bit
	 * bit 4 = master (0) or slave (1)
	 */
	ports_outb(ATA_PORT_PRIMARY_DRIVE_SELECT, 0 << 4);

	uint8_t status;
	do {
		status = ports_inb(ATA_PORT_PRIMARY_STATUS);
	} while((status & ATA_STATUS_BUSY) || (status & ATA_STATUS_READY) == 0);
}

void ata_pio_read(uint32_t lba, uint8_t count, volatile void *buffer) {
	ata_pio_begin();

	ports_outb(ATA_PORT_PRIMARY_DRIVE_SELECT, (uint8_t)(lba >> 24) | (1 << 6)); /* set bit 6 for LBA mode */
	ports_outb(ATA_PORT_PRIMARY_SECTOR_COUNT, count);
	ports_outb(ATA_PORT_PRIMARY_LBA_LOW, (uint8_t)lba);
	ports_outb(ATA_PORT_PRIMARY_LBA_MID, (uint8_t)(lba >> 8));
	ports_outb(ATA_PORT_PRIMARY_LBA_HIGH, (uint8_t)(lba >> 16));
	ports_outb(ATA_PORT_PRIMARY_COMMAND, ATA_COMMAND_PIO_READ_RETRY);

	volatile uint16_t *ptr = buffer;
	for(;;) {
		while(ports_inb(ATA_PORT_PRIMARY_ALT_STATUS) & ATA_STATUS_BUSY) {}

		uint8_t status = ports_inb(ATA_PORT_PRIMARY_STATUS);
		if(status & ATA_STATUS_ERROR) { kernel_panic("ATA: error status"); }

		if(status & ATA_STATUS_DATA_REQUEST) {
			ports_str_ins(ATA_PORT_PRIMARY_DATA, ptr, 256u);
			ptr += 256u;
		} else { break; }
	}
}
