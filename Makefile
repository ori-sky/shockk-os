export TOOLCHAIN_PREFIX=$(CURDIR)/toolchain/prefix
export TARGET=i386-elf

export TARGET_ASM=$(TOOLCHAIN_PREFIX)/bin/nasm
export TARGET_CC =$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc
export TARGET_CXX=$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-g++
export TARGET_LD =$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc
export QEMU=qemu-system-i386

export BOOTSECTOR_BIN=$(CURDIR)/bootsector.bin
export LOADER_ELF=$(CURDIR)/loader.elf
export KERNEL_ELF=$(CURDIR)/kernel.elf

export INCLUDE_PATHS=$(CURDIR)/include
export CXXWARNS=-Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wformat=2 -Winit-self -Wmissing-include-dirs -Wredundant-decls -Wstrict-overflow=5 -Wundef -Wdisabled-optimization -Wsign-conversion -Wstack-protector -Wabi -Winline -Wpadded -Wswitch-enum

IMAGE=$(CURDIR)/shk.img
MNTDIR=$(CURDIR)/mnt
BOOTDIR=$(CURDIR)/boot

.PHONY: all
all: image

.PHONY: toolchain
toolchain:
	$(MAKE) -C toolchain

.PHONY: $(BOOTSECTOR_BIN)
$(BOOTSECTOR_BIN): toolchain
	$(MAKE) -C boot $@

.PHONY: $(LOADER_ELF)
$(LOADER_ELF): toolchain
	$(MAKE) -C loader $@

.PHONY: $(KERNEL_ELF)
$(KERNEL_ELF): toolchain
	$(MAKE) -C kernel $@

.PHONY: clean-image
clean-image:
	rm -fv $(IMAGE)

.PHONY: image
image: $(BOOTSECTOR_BIN) $(LOADER_ELF) $(KERNEL_ELF)
	dd bs=512 count=65536 if=/dev/zero of=$(IMAGE)
	mkfs.ext2 -F $(IMAGE)
	dd bs=512 count=2 if=$(BOOTSECTOR_BIN) of=$(IMAGE) conv=notrunc
	e2cp -v $(LOADER_ELF) $(IMAGE):/boot/loader.elf
#	mkdiskimage syslinux.img 80 2 18
#	syslinux --offset 9216 syslinux.img
#	mcopy -i syslinux.img@@9K ::ldlinux.sys ldlinux.sys
#	mcopy -i syslinux.img@@9K ::ldlinux.c32 ldlinux.c32
#
#	# 31 MB so that our final disk can be 32 MB with partition table preamble
#	dd bs=512 count=63488 if=/dev/zero of=$(IMAGE).tmp
#	mkfs.ext2 -F $(IMAGE).tmp
#	e2cp -v ldlinux.sys $(IMAGE).tmp:/boot/extlinux.sys
#	e2cp -v ldlinux.c32 $(IMAGE).tmp:/boot/extlinux/ldlinux.c32
#	e2cp -v syslinux.cfg $(IMAGE).tmp:/boot/extlinux/extlinux.conf
#
#	dd bs=512 count=65536 if=/dev/zero of=$(IMAGE)
#	sh $(CURDIR)/scripts/partition_image.sh
#	dd if=/usr/lib/EXTLINUX/mbr.bin of=$(IMAGE) bs=440 count=1 conv=notrunc
#	dd if=$(IMAGE).tmp of=$(IMAGE) bs=512 count=63488 seek=1048576 oflag=seek_bytes

#	losetup /dev/loop0 $(IMAGE)
#	losetup /dev/loop1 $(IMAGE) -o 1048576 # first partition at 1MB
#	mkfs.ext2 /dev/loop1
#	mkdir -pv $(MNTDIR)
#	mount -t ext2 /dev/loop1 $(MNTDIR)
#	install -v -o root -g root -m 644 -D $(LOADER_ELF) $(MNTDIR)/boot/loader
#	install -v -o root -g root -m 644 -D $(KERNEL_ELF) $(MNTDIR)/boot/kernel
#	install -v -o root -g root -m 644 -D $(BOOTDIR)/grub.cfg $(MNTDIR)/boot/grub/grub.cfg
#	grub-install --target=i386-pc --root-directory=$(MNTDIR) --no-floppy --modules="normal part_msdos ext2 multiboot configfile" /dev/loop0
#	$(MAKE) clean-image-mounts
#	chown $$SUDO_UID:$$SUDO_GID $(IMAGE)

.PHONY: qemu
qemu:
	$(QEMU) $(IMAGE)

.PHONY: clean
clean:
	$(MAKE) -C boot clean
	$(MAKE) -C loader clean
	$(MAKE) -C kernel clean

.PHONY: toolchain-clean
toolchain-clean:
	$(MAKE) -C toolchain clean

.PHONY: toolchain-distclean
toolchain-distclean:
	$(MAKE) -C toolchain distclean
