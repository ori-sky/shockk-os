#include <kernel/ports.h>
#include <kernel/ata.h>

void ata_read(uint32_t lba, uint8_t count, void *buffer) {
	ports_outb(ATA_PORT_PRIMARY_DRIVE_SELECT, (uint8_t)(lba >> 24) | (1 << 6)); /* set bit 6 for LBA mode */
	ports_outb(ATA_PORT_PRIMARY_SECTOR_COUNT, count);
	ports_outb(ATA_PORT_PRIMARY_LBA_1, (uint8_t)lba);
	ports_outb(ATA_PORT_PRIMARY_LBA_2, (uint8_t)(lba >> 8));
	ports_outb(ATA_PORT_PRIMARY_LBA_3, (uint8_t)(lba >> 16));
	ports_outb(ATA_PORT_PRIMARY_COMMAND, ATA_COMMAND_READ_RETRY);
	while((ports_inb(ATA_PORT_PRIMARY_COMMAND) & (1 << 3)) == 0) {} /* wait until sector buffer is ready */
	ports_str_ins(buffer, 256u * count);
}
