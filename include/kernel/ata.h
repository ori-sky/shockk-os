#ifndef ATA_H
#define ATA_H

#define ATA_PORT_PRIMARY_SECTOR_COUNT 0x1F2
#define ATA_PORT_PRIMARY_LBA_1        0x1F3
#define ATA_PORT_PRIMARY_LBA_2        0x1F4
#define ATA_PORT_PRIMARY_LBA_3        0x1F5
#define ATA_PORT_PRIMARY_DRIVE_SELECT 0x1F6
#define ATA_PORT_PRIMARY_COMMAND      0x1F7

#define ATA_COMMAND_READ_RETRY 0x20

void ata_read(uint32_t, uint8_t, void *);

#endif
