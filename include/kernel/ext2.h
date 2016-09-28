#ifndef EXT2_H
#define EXT2_H

#include <stdint.h>

class Ext2 {
public:
	struct Superblock {
		struct Base {
			uint32_t inode_count;
			uint32_t block_count;
			uint32_t su_reserved_block_count;
			uint32_t unalloc_block_count;
			uint32_t unalloc_inode_count;
			uint32_t superblock_id;
			uint32_t block_shift;
			uint32_t fragment_shift;
			uint32_t group_block_count;
			uint32_t group_fragment_count;
			uint32_t group_inode_count;
			uint32_t last_mount_time;
			uint32_t last_write_time;
			uint16_t mounts_since_check;
			uint16_t mounts_until_check;
			uint16_t signature;
			uint16_t state;
			uint16_t error_method;
			uint16_t version_minor;
			uint32_t check_time;
			uint32_t check_interval;
			uint32_t os;
			uint32_t version_major;
			uint16_t su_id;
			uint16_t sg_id;
		} __attribute__((packed));
		struct Ext {
			uint32_t first_unreserved;
			uint16_t inode_size;
			uint16_t group;
			uint32_t optional_features;
			uint32_t required_features;
			uint32_t write_features;
			uint8_t  fs_id[16];
			char     volume_name[16];
			char     last_mount_path[64];
			uint32_t compression_algos;
			uint8_t  block_prealloc_file_count;
			uint8_t  block_prealloc_dir_count;
			uint8_t  unused1[2];
			uint8_t  journal_id[16];
			uint32_t journal_inode;
			uint32_t journal_device;
			uint32_t orphan_head;
			uint8_t  unused2[788];
		} __attribute__((packed));
		Base base;
		Ext  ext;
	} __attribute__((packed));
	struct GroupDesc {
		uint32_t block_usage_bitmap_addr;
		uint32_t inode_usage_bitmap_addr;
		uint32_t inode_table_addr;
		uint16_t unalloc_block_count;
		uint16_t unalloc_inode_count;
		uint16_t directory_count;
		uint8_t  unused[14];
	} __attribute__((packed));
	struct Inode {
		uint16_t type        :  4;
		uint16_t permissions : 12;
		uint16_t uid;
		uint32_t size_low;
		uint32_t last_access_time;
		uint32_t creation_time;
		uint32_t last_modify_time;
		uint32_t deletion_time;
		uint32_t gid;
		uint16_t hard_link_count;
		uint32_t disk_sector_count;
		uint32_t flags;
		uint8_t  os_specific_1[4];
		uint32_t block_ptr[12];
		uint32_t block_ptr_singly;
		uint32_t block_ptr_doubly;
		uint32_t block_ptr_triply;
		uint32_t generation;
		uint32_t acl;
		uint32_t size_high;
		uint32_t fragment_block_addr;
		uint8_t  os_specific_2[12];
	} __attribute__((packed));
private:
	uint32_t lba;
	Superblock superblock;
public:
	Ext2(uint32_t lba);
	uint32_t GetBlockSize(void);
	uint32_t GetBlockOffset(uint32_t block_id);
	GroupDesc GetGroupDesc(uint32_t group_id);
	void GetInode(uint32_t inode_id);
};

void read_inode(uint32_t inode);

#endif
