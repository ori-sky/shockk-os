#include <stddef.h>
#include <kernel/ext2.h>
#include <kernel/ata.h>
#include <kernel/itoa.h>
#include <kernel/panic.h>

Ext2::Ext2(uint32_t lba) : lba(lba) {
	ata_pio_read(lba + 1024 / 512, 2, &this->superblock);
	if(this->superblock.base.signature != 0xef53) {
		kernel_panic("invalid ext2 superblock signature");
	}
}

uint32_t Ext2::GetBlockSize(void) {
	return 1024u << this->superblock.base.block_shift;
}

uint32_t Ext2::GetBlockOffset(uint32_t block_id) {
	return block_id * this->GetBlockSize();
}

Ext2::GroupDesc Ext2::GetGroupDesc(uint32_t group_id) {
	uint32_t block_id = this->superblock.base.superblock_id + 1;
	uint32_t offset = this->GetBlockOffset(block_id) + group_id * sizeof(GroupDesc);

	char buffer[512];
	ata_pio_read(this->lba + offset / 512, 1, &buffer);

	GroupDesc gd;
	char *ptr = reinterpret_cast<char *>(&gd);
	for(size_t i = 0; i < sizeof(GroupDesc); ++i) {
		ptr[i] = buffer[i];
	}

	return gd;
}

void Ext2::GetInode(uint32_t inode_id) {
	uint32_t group_id = (inode_id - 1) / this->superblock.base.group_inode_count;
	GroupDesc gd = this->GetGroupDesc(group_id);

	uint32_t index = (inode_id - 1) % this->superblock.base.group_inode_count;

	// size of inode == 128 bytes?
	uint32_t inode_addr = gd.inode_table_addr * this->GetBlockSize() + index * 128;

	char sz[12] = {'0', 'x', 0};
	uitoa(inode_addr, &sz[2], 16);
	kernel_panic(sz);
}
