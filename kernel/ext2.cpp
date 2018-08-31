#include <kernel/ext2.h>
#include <kernel/ata.h>
#include <kernel/panic.h>

Ext2::Ext2(Pager *pager, uint32_t lba) : pager(pager), lba(lba) {
	ata_pio_read(lba + 1024 / 512, 2, &this->superblock);
	if(this->superblock.base.signature != SUPERBLOCK_SIGNATURE) {
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

uint32_t Ext2::GetGroupDescAddr(uint32_t group_id) {
	uint32_t block_id = this->superblock.base.superblock_id + 1;
	return this->GetBlockAddr(block_id) + group_id * sizeof(GroupDesc);
}
#include <kernel/itoa.h>
uint32_t Ext2::GetInodeBlockAddr(Inode inode, uint32_t block_ptr_id) {
	uint32_t block_size = this->GetBlockSize();
	uint32_t block_num_ptrs = block_size / sizeof(uint32_t);
	uint32_t block_addr;

	if(block_ptr_id < 12) {
		block_addr = this->GetBlockAddr(inode.block_ptr[block_ptr_id]);
	} else if(block_ptr_id < 12 + block_num_ptrs) {
		uint32_t singly_addr = this->GetBlockAddr(inode.block_ptr_singly);
		uint32_t singly_offset = (block_ptr_id - 12) * sizeof(uint32_t);
		ata_pio_read_bytes(this->lba * 512 + singly_addr + singly_offset, &block_addr);
	} else {
		kernel_panic("doubly indirect ptrs not implemented yet");
	}

	return block_addr;
}

Ext2::GroupDesc Ext2::GetGroupDesc(uint32_t group_id) {
	uint32_t offset = this->GetGroupDescAddr(group_id);
	GroupDesc gd;
	ata_pio_read_bytes(this->lba * 512 + offset, &gd);
	return gd;
}

Maybe<Ext2::Inode> Ext2::GetInode(uint32_t inode_id) {
	uint32_t inode_addr = this->GetInodeAddr(inode_id);
	Inode inode;
	ata_pio_read_bytes(this->lba * 512 + inode_addr, &inode);
	return Maybe<Inode>(inode);
}

Maybe<Ext2::Inode> Ext2::GetInode(Inode &pwd, const char *name) {
	// XXX: this assumes all dir entries will fit in a single block, which is bad
	auto mDirent = this->GetDirectoryEntry(pwd.block_ptr[0], name);
	if(mDirent.IsNothing()) {
		return Maybe<Inode>();
	} else {
		return this->GetInode(mDirent.FromJust().inode_id);
	}
}

Maybe<Ext2::Inode> Ext2::GetInode(size_t count, const char *paths[]) {
	auto mRoot = this->GetInode(ROOT_INODE);
	if(mRoot.IsNothing()) { return Maybe<Inode>(); }

	Inode ref = mRoot.FromJust();

	for(size_t i = 0; i < count; ++i) {
		auto mRef = this->GetInode(ref, paths[i]);
		if(mRef.IsNothing()) { return Maybe<Inode>(); }
		ref = mRef.FromJust();
	}

	return ref;
}

Maybe<Ext2::DirectoryEntry> Ext2::GetDirectoryEntry(uint32_t block_id, const char *name) {
	uint32_t block_size = this->GetBlockSize();
	uint32_t block_addr = this->GetBlockAddr(block_id);

	// XXX: this assumes a block is 1024 bytes, which is very bad if it's not
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

#include <kernel/screen.h>
void Ext2::ReadInode(Inode inode, uint32_t offset, size_t count, char *ptr) {
	uint32_t block_size = this->GetBlockSize();
	size_t offset_end = offset + count;
	size_t block_ptr_begin = offset / block_size;
	size_t block_ptr_end = offset_end / block_size + (offset_end % block_size != 0) + 1;

	size_t n = 0;
	size_t off = offset % block_size;

	for(size_t b = block_ptr_begin; b < block_ptr_end; ++b) {
		uint32_t block_ptr = this->GetInodeBlockAddr(inode, b);
		char buffer[1024]; // XXX: assuming block size is 1024 again
		ata_pio_read(this->lba + block_ptr / 512, block_size / 512, &buffer);
		while(off < block_size && n < count) {
			ptr[n++] = buffer[off++];
		}
		off = 0;
	}
}
