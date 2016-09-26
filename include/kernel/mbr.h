#ifndef MBR_H
#define MBR_H

struct MBREntry {
	uint8_t  boot_indicator;
	uint8_t  starting_head;
	uint8_t  starting_sector   :  6;
	uint8_t  starting_cylinder : 10;
	uint8_t  system_id;
	uint8_t  ending_head;
	uint8_t  ending_sector     :  6;
	uint8_t  ending_cylinder   : 10;
	uint32_t starting_lba;
	uint32_t sector_count;
} __attribute__((packed));

struct MBR {
	uint8_t padding[0x1BE];
	MBREntry entries[4];
} __attribute__((packed));

MBR mbr_read(void);

#endif
