export TOOLCHAIN_PREFIX=$(CURDIR)/toolchain/prefix
export TARGET=i386-elf

export TARGET_ASM=$(TOOLCHAIN_PREFIX)/bin/nasm
export TARGET_CC =$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc
export TARGET_CXX=$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-g++
export TARGET_LD =$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc
export QEMU=qemu-system-i386

export LOADER_ELF=$(CURDIR)/loader.elf
export KERNEL_ELF=$(CURDIR)/kernel.elf

export INCLUDE_PATHS=$(CURDIR)/include
export CXXWARNS=-Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wformat=2 -Winit-self -Wmissing-include-dirs -Wredundant-decls -Wstrict-overflow=5 -Wundef -Wdisabled-optimization -Wsign-conversion -Wstack-protector -Wabi -Winline -Wpadded -Wswitch-enum

IMAGE=$(CURDIR)/shk.img
MNTDIR=$(CURDIR)/mnt
BOOTDIR=$(CURDIR)/boot

.PHONY: all
all: $(LOADER_ELF) $(KERNEL_ELF)

.PHONY: toolchain
toolchain:
	$(MAKE) -C toolchain

.PHONY: $(LOADER_ELF)
$(LOADER_ELF): toolchain
	$(MAKE) -C loader $@

.PHONY: $(KERNEL_ELF)
$(KERNEL_ELF): toolchain
	$(MAKE) -C kernel $@

.PHONY: clean-image
clean-image: clean-image-mounts
	rm -fv $(IMAGE)

.PHONY: clean-image-mounts
clean-image-mounts:
	-umount $(MNTDIR)
	-rmdir -v $(MNTDIR)
	-losetup -d /dev/loop1
	-losetup -d /dev/loop0

.PHONY: image
image: clean-image
	dd bs=512 count=65536 if=/dev/zero of=$(IMAGE)
	sh $(CURDIR)/scripts/partition_image.sh
	losetup /dev/loop0 $(IMAGE)
	losetup /dev/loop1 $(IMAGE) -o 1048576 # first partition at 1MB
	mkfs.ext2 /dev/loop1
	mkdir -pv $(MNTDIR)
	mount -t ext2 /dev/loop1 $(MNTDIR)
	install -v -o root -g root -m 644 -D $(LOADER_ELF) $(MNTDIR)/boot/loader
	install -v -o root -g root -m 644 -D $(KERNEL_ELF) $(MNTDIR)/boot/kernel
	install -v -o root -g root -m 644 -D $(BOOTDIR)/grub.cfg $(MNTDIR)/boot/grub/grub.cfg
	grub-install --target=i386-pc --root-directory=$(MNTDIR) --no-floppy --modules="normal part_msdos ext2 multiboot configfile" /dev/loop0
	$(MAKE) clean-image-mounts
	chown $$SUDO_UID:$$SUDO_GID $(IMAGE)

.PHONY: qemu
qemu:
	$(QEMU) $(IMAGE)

.PHONY: clean
clean:
	$(MAKE) -C loader clean
	$(MAKE) -C kernel clean

.PHONY: toolchain-clean
toolchain-clean:
	$(MAKE) -C toolchain clean

.PHONY: toolchain-distclean
toolchain-distclean:
	$(MAKE) -C toolchain distclean
