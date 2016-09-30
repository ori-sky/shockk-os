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

uint32_t Ext2::GetBlockAddr(uint32_t block_id) {
	return block_id * this->GetBlockSize();
}

uint32_t Ext2::GetInodeAddr(uint32_t inode_id) {
	uint32_t group_id = (inode_id - 1) / this->superblock.base.group_inode_count;
	GroupDesc gd = this->GetGroupDesc(group_id);

	uint32_t index = (inode_id - 1) % this->superblock.base.group_inode_count;
	return gd.inode_table_addr * this->GetBlockSize() + index * sizeof(Inode);
}

Ext2::GroupDesc Ext2::GetGroupDesc(uint32_t group_id) {
	uint32_t block_id = this->superblock.base.superblock_id + 1;
	uint32_t offset = this->GetBlockAddr(block_id) + group_id * sizeof(GroupDesc);

	char buffer[1024];
	ata_pio_read(this->lba + offset / 512, 2, &buffer);

	GroupDesc *gd = reinterpret_cast<GroupDesc *>(&buffer[offset % 512]);
	return *gd;
}

Maybe<Ext2::Inode> Ext2::GetInode(uint32_t inode_id) {
	uint32_t inode_addr = this->GetInodeAddr(inode_id);

	char buffer[1024];
	ata_pio_read(this->lba + inode_addr / 512, 2, &buffer);

	Inode *inode = reinterpret_cast<Inode *>(&buffer[inode_addr % 512]);
	return Maybe<Inode>(*inode);
}

Maybe<Ext2::Inode> Ext2::GetInode(Inode &pwd, const char *name) {
	auto mDirent = this->GetDirectoryEntry(pwd.block_ptr[0], name);
	if(mDirent.IsNothing()) {
		return Maybe<Inode>();
	} else {
		return this->GetInode(mDirent.FromJust().inode_id);
	}
}

Maybe<Ext2::DirectoryEntry> Ext2::GetDirectoryEntry(uint32_t block_id, const char *name) {
	uint32_t block_size = this->GetBlockSize();
	uint32_t block_addr = this->GetBlockAddr(block_id);

	char buffer[1024];
	ata_pio_read(this->lba + block_addr / 512, 2, &buffer);

	DirectoryEntry *dirent;
	for(size_t offset = 0; offset < block_size; offset += dirent->size) {
		dirent = reinterpret_cast<DirectoryEntry *>(&buffer[offset]);
		if(dirent->inode_id == 0) { break; }

		bool match = true;
		for(size_t n = 0; n < dirent->len; ++n) {
			if(buffer[offset + sizeof(DirectoryEntry) + n] != name[n]) {
				match = false;
				break;
			}
		}
		if(match) {
			return Maybe<DirectoryEntry>(*dirent);
		}
	}
	return Maybe<DirectoryEntry>();
}
