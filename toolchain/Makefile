NASM_VERSION=2.12.02
BINUTILS_VERSION=2.31
GCC_VERSION=8.2.0
GDB_VERSION=8.2

.PHONY: all
all: nasm binutils gcc gdb

.PHONY: nasm
nasm: $(TOOLCHAIN_PREFIX)/bin/nasm

.PHONY: binutils
binutils: $(TOOLCHAIN_PREFIX)/bin/$(TARGET)-ar

.PHONY: gcc
gcc: $(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc

.PHONY: gdb
gdb: $(TOOLCHAIN_PREFIX)/bin/gdb

# downloads

nasm-$(NASM_VERSION).tar.gz:
	wget http://www.nasm.us/pub/nasm/releasebuilds/$(NASM_VERSION)/$@

binutils-$(BINUTILS_VERSION).tar.gz:
	wget ftp://ftp.gnu.org/gnu/binutils/$@

gcc-$(GCC_VERSION).tar.gz:
	wget ftp://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/$@

gdb-$(GDB_VERSION).tar.gz:
	wget ftp://ftp.gnu.org/gnu/gdb/$@

# patches

nasm-$(NASM_VERSION): nasm-$(NASM_VERSION).tar.gz
	tar -xvf $<

binutils-$(BINUTILS_VERSION): binutils-$(BINUTILS_VERSION).tar.gz
	tar -xvf $<
	patch -p0 < shk-binutils-$(BINUTILS_VERSION).patch

gcc-$(GCC_VERSION): gcc-$(GCC_VERSION).tar.gz
	tar -xvf $<
	patch -p0 < shk-gcc-$(GCC_VERSION).patch

gdb-$(GDB_VERSION): gdb-$(GDB_VERSION).tar.gz
	tar -xvf $<

# gcc prerequisites

gcc-$(GCC_VERSION)/gmp: gcc-$(GCC_VERSION)
	cd $< && sh contrib/download_prerequisites
	touch $@

# nasm build

build-nasm-$(NASM_VERSION): nasm-$(NASM_VERSION)
	mkdir -pv $@
	cd $@ && ../$</configure --prefix="$(TOOLCHAIN_PREFIX)"

$(TOOLCHAIN_PREFIX)/bin/nasm: build-nasm-$(NASM_VERSION)
	$(MAKE) -C $<
	$(MAKE) -C $< install

# hosted binutils build

build-binutils-$(BINUTILS_VERSION): binutils-$(BINUTILS_VERSION)
	mkdir -pv $@
	cd $@ && ../$</configure --target=$(TARGET) --prefix="$(TOOLCHAIN_PREFIX)" --with-sysroot=$(SYSROOT) --disable-werror

$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-ar: build-binutils-$(BINUTILS_VERSION)
	$(MAKE) -C $<
	$(MAKE) -C $< install

# hosted gcc build

build-gcc-$(GCC_VERSION): gcc-$(GCC_VERSION) gcc-$(GCC_VERSION)/gmp $(TOOLCHAIN_PREFIX)/bin/$(TARGET)-ar
	mkdir -pv $@
	cd $@ && ../$</configure --target=$(TARGET) --prefix="$(TOOLCHAIN_PREFIX)" --with-sysroot=$(SYSROOT) --enable-languages=c,c++

$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc: build-gcc-$(GCC_VERSION) $(SYSROOT)/usr/include/stdio.h
	$(MAKE) -C $< all-gcc
	$(MAKE) -C $< all-target-libgcc
	$(MAKE) -C $< install-gcc
	$(MAKE) -C $< install-target-libgcc

# gdb build

build-gdb-$(GDB_VERSION): gdb-$(GDB_VERSION)
	mkdir -pv $@

$(TOOLCHAIN_PREFIX)/bin/gdb: build-gdb-$(GDB_VERSION)

.PHONY: clean
clean:
	rm -rfv "$(TOOLCHAIN_PREFIX)"
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION) clean
	$(MAKE) -C build-gcc-$(GCC_VERSION) clean
	$(MAKE) -C build-gdb-$(GDB_VERSION) clean

.PHONY: distclean
distclean:
	rm -rfv "$(TOOLCHAIN_PREFIX)"
	rm -rfv "build-nasm-$(NASM_VERSION)"
	rm -rfv "build-binutils-$(BINUTILS_VERSION)"
	rm -rfv "build-gcc-$(GCC_VERSION)"
	rm -rfv "build-gdb-$(GDB_VERSION)"
