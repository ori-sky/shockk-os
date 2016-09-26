export LOADER_ELF=$(CURDIR)/loader.elf
export KERNEL_ELF=$(CURDIR)/kernel.elf

IMAGE=$(CURDIR)/shk.img
MNTDIR=$(CURDIR)/mnt
BOOTDIR=$(CURDIR)/boot

export ASM=nasm
export CC=i386-elf-gcc
export CXX=i386-elf-g++
export LD=i386-elf-gcc

export INCLUDE_PATHS=$(CURDIR)/include
export CXXWARNS=-Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wformat=2 -Winit-self -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef -Wdisabled-optimization -Wsign-conversion -Wstack-protector -Wabi -Waggregate-return -Winline -Wpadded -Wswitch-enum

.PHONY: all
all: $(LOADER_ELF) $(KERNEL_ELF)

.PHONY: image
image: all
	rm -fv $(IMAGE)
	dd bs=512 count=65536 if=/dev/zero of=$(IMAGE)
	sh $(CURDIR)/scripts/partition_image.sh
	losetup /dev/loop0 $(IMAGE)
	losetup /dev/loop1 $(IMAGE) -o 1048576 # first partition at 1MB
	mkfs.ext2 /dev/loop1
	mkdir -pv $(MNTDIR)
	mount /dev/loop1 $(MNTDIR)
	install -v -o root -g root -m 644 -D $(LOADER_ELF) $(MNTDIR)/boot/loader
	install -v -o root -g root -m 644 -D $(KERNEL_ELF) $(MNTDIR)/boot/kernel
	install -v -o root -g root -m 644 -D $(BOOTDIR)/grub.cfg $(MNTDIR)/boot/grub/grub.cfg
	grub-install --target=i386-pc --root-directory=$(MNTDIR) --no-floppy --modules="normal part_msdos ext2 multiboot configfile" /dev/loop0
	umount $(MNTDIR)
	rmdir -v $(MNTDIR)
	losetup -d /dev/loop1
	losetup -d /dev/loop0
	chown $$SUDO_UID:$$SUDO_GID $(IMAGE)

.PHONY: $(LOADER_ELF)
$(LOADER_ELF):
	$(MAKE) -C loader $@

.PHONY: $(KERNEL_ELF)
$(KERNEL_ELF):
	$(MAKE) -C kernel $@

.PHONY: clean
clean:
	make -C loader clean
	make -C kernel clean
