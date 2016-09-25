#include <stddef.h>
#include <kernel/ext2.h>
#include <kernel/ata.h>
#include <kernel/itoa.h>
#include <kernel/panic.h>

uint32_t block_addr(Ext2Superblock *sb, uint32_t block_id) {
	uint32_t block_size = 1024u << sb->base.block_shift;
	return block_id * block_size;
}

Ext2Superblock read_superblock(void) {
	Ext2Superblock sb;
	ata_pio_read(1024 / 512, 2, &sb);
	return sb;
}

Ext2GroupDesc read_group_desc(Ext2Superblock *sb, uint32_t group_id) {
	uint32_t block_id = sb->base.superblock_id + 1;
	uint32_t addr = block_addr(sb, block_id) + group_id * sizeof(Ext2GroupDesc);

	char buffer[512];
	ata_pio_read(addr / 512, 1, &buffer);

	Ext2GroupDesc gd;
	char *ptr = reinterpret_cast<char *>(&gd);
	for(size_t i = 0; i < sizeof(Ext2GroupDesc); ++i) {
		ptr[i] = buffer[i];
	}

	return gd;
}

void read_inode(uint32_t inode_id) {
	Ext2Superblock sb = read_superblock();

	if(sb.base.signature != 0xef53) {
		kernel_panic("Failed to read superblock");
	}

	uint32_t group_id = (inode_id - 1) / sb.base.group_inode_count;
	kernel_panic("test");
	Ext2GroupDesc gd = read_group_desc(&sb, group_id);

	uint32_t block_size = 1024u << sb.base.block_shift;
	uint32_t index    = (inode_id - 1) % sb.base.group_inode_count;

	// size of inode == 128 bytes?
	uint32_t inode_addr = gd.inode_table_addr * block_size + index * 128;

	char sz[12];
	uitoa(inode_addr, sz, 16);
	kernel_panic(sz);
}
