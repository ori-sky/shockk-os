#include <kernel/ports.h>
#include <kernel/ata.h>

void ata_read(uint32_t lba, uint8_t count, volatile void *buffer) {
	ports_outb(ATA_PORT_PRIMARY_DRIVE_SELECT, (uint8_t)(lba >> 24) | (1 << 6)); /* set bit 6 for LBA mode */
	ports_outb(ATA_PORT_PRIMARY_SECTOR_COUNT, count);
	ports_outb(ATA_PORT_PRIMARY_LBA_LOW, (uint8_t)lba);
	ports_outb(ATA_PORT_PRIMARY_LBA_MID, (uint8_t)(lba >> 8));
	ports_outb(ATA_PORT_PRIMARY_LBA_HIGH, (uint8_t)(lba >> 16));
	ports_outb(ATA_PORT_PRIMARY_COMMAND, ATA_COMMAND_PIO_READ_RETRY);

	while((ports_inb(ATA_PORT_PRIMARY_ALT_STATUS) & (1 << 3)) == 0) {} /* wait until drive has PIO data to transfer */
	ports_str_ins(ATA_PORT_PRIMARY_DATA, buffer, 256u * count);
}
