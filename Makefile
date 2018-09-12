export TOOLCHAIN_BARE=$(CURDIR)/toolchain/bare
export TOOLCHAIN_HOSTED=$(CURDIR)/toolchain/hosted
export TOOLCHAIN_SYSROOT=$(CURDIR)/toolchain/sysroot
export TARGET=i386-shk

export SHK_LIBC_SOURCE_DIR=$(CURDIR)/libc
export SHK_INCLUDE_DIR=$(CURDIR)/include

export TARGET_ASM=$(TOOLCHAIN_HOSTED)/bin/nasm
export TARGET_CC =$(TOOLCHAIN_HOSTED)/bin/$(TARGET)-gcc
export TARGET_CXX=$(TOOLCHAIN_HOSTED)/bin/$(TARGET)-g++
export TARGET_LD =$(TOOLCHAIN_HOSTED)/bin/$(TARGET)-gcc
export TARGET_AR =$(TOOLCHAIN_HOSTED)/bin/$(TARGET)-ar

export QEMU=qemu-system-i386

export SHKBOOT_BIN=$(CURDIR)/shkboot.bin
export LOADER_ELF=$(CURDIR)/loader.elf
export KERNEL_ELF=$(CURDIR)/kernel.elf
export CRT0_O=$(CURDIR)/crt0.o
export LIBC_A=$(CURDIR)/libc.a

export INCLUDE_PATHS=$(CURDIR)/include
export CXXWARNS=-Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wformat=2 -Winit-self -Wmissing-include-dirs -Wredundant-decls -Wstrict-overflow=5 -Wundef -Wdisabled-optimization -Wsign-conversion -Wstack-protector -Winline -Wpadded -Wswitch-enum

IMAGE=$(CURDIR)/shk.img
MNTDIR=$(CURDIR)/mnt
BOOTDIR=$(CURDIR)/boot

.PHONY: all
all: image

.PHONY: toolchain
toolchain:
	$(MAKE) -C toolchain

.PHONY: $(SHKBOOT_BIN)
$(SHKBOOT_BIN): toolchain
	$(MAKE) -C boot $@

.PHONY: $(LOADER_ELF)
$(LOADER_ELF): toolchain
	$(MAKE) -C loader $@

.PHONY: $(KERNEL_ELF)
$(KERNEL_ELF): toolchain
	$(MAKE) -C kernel $@

.PHONY: $(CRT0_O)
$(CRT0_O): toolchain
	$(MAKE) -C libc $@

.PHONY: $(LIBC_A)
$(LIBC_A): toolchain
	$(MAKE) -C libc $@

.PHONY: libc
libc: $(CRT0_O) $(LIBC_A)

.PHONY: test
test:
	$(MAKE) -C test all

.PHONY: clean-image
clean-image:
	rm -fv $(IMAGE)

.PHONY: image
image: $(SHKBOOT_BIN) $(LOADER_ELF) $(KERNEL_ELF)
	dd bs=512 count=65536 if=/dev/zero of=$(IMAGE)
	mkfs.ext2 -F $(IMAGE)
	dd bs=512 count=2 if=$(SHKBOOT_BIN) of=$(IMAGE) conv=notrunc
	e2cp -v $(LOADER_ELF) $(IMAGE):/boot/loader.elf
	e2cp -v $(KERNEL_ELF) $(IMAGE):/boot/kernel.elf

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
