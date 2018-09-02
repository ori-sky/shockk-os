NASM_VERSION=2.12.02
BINUTILS_VERSION=2.31
GCC_VERSION=8.2.0
NEWLIB_VERSION=3.0.0

.PHONY: all
all: nasm gcc newlib

.PHONY: nasm
nasm: $(TOOLCHAIN_PREFIX)/bin/nasm

.PHONY: binutils
binutils: $(TOOLCHAIN_PREFIX)/bin/$(TARGET)-ar

.PHONY: gcc
gcc: $(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc

.PHONY: newlib
newlib: $(TOOLCHAIN_SYSROOT)/$(TARGET)/lib/libc.a

# downloads

nasm-$(NASM_VERSION).tar.gz:
	wget http://www.nasm.us/pub/nasm/releasebuilds/$(NASM_VERSION)/nasm-$(NASM_VERSION).tar.gz

binutils-$(BINUTILS_VERSION).tar.gz:
	wget ftp://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VERSION).tar.gz

gcc-$(GCC_VERSION).tar.gz:
	wget ftp://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.gz

newlib-$(NEWLIB_VERSION).tar.gz:
	wget ftp://sourceware.org/pub/newlib/newlib-$(NEWLIB_VERSION).tar.gz

# patches

nasm-$(NASM_VERSION): nasm-$(NASM_VERSION).tar.gz
	tar -xvf $<

binutils-$(BINUTILS_VERSION): binutils-$(BINUTILS_VERSION).tar.gz
	tar -xvf $<
	patch -p0 < shk-binutils-$(BINUTILS_VERSION).patch

gcc-$(GCC_VERSION): gcc-$(GCC_VERSION).tar.gz
	tar -xvf $<
	patch -p0 < shk-gcc-$(GCC_VERSION).patch

newlib-$(NEWLIB_VERSION): newlib-$(NEWLIB_VERSION).tar.gz
	tar -xvf $<
	patch -p0 < shk-newlib-$(NEWLIB_VERSION).patch

# builds

$(TOOLCHAIN_PREFIX)/bin/nasm: nasm-$(NASM_VERSION)
	mkdir -pv build-nasm-$(NASM_VERSION)
	cd build-nasm-$(NASM_VERSION) && ../$</configure --prefix="$(TOOLCHAIN_PREFIX)"
	$(MAKE) -C build-nasm-$(NASM_VERSION)
	$(MAKE) -C build-nasm-$(NASM_VERSION) install

$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-ar: binutils-$(BINUTILS_VERSION)
	mkdir -pv build-binutils-$(BINUTILS_VERSION)
	cd build-binutils-$(BINUTILS_VERSION) && ../$</configure --target=$(TARGET) --prefix="$(TOOLCHAIN_PREFIX)" --with-sysroot --disable-nls --disable-werror
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION)
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION) install

gcc-$(GCC_VERSION)/gmp: gcc-$(GCC_VERSION)
	cd gcc-$(GCC_VERSION) && sh contrib/download_prerequisites
	touch $@

$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc: gcc-$(GCC_VERSION)/gmp $(TOOLCHAIN_PREFIX)/bin/$(TARGET)-ar
	mkdir -pv build-gcc-$(GCC_VERSION)
	cd build-gcc-$(GCC_VERSION) && ../gcc-$(GCC_VERSION)/configure --target=$(TARGET) --prefix="$(TOOLCHAIN_PREFIX)" --disable-nls --enable-languages=c,c++ --without-headers
	$(MAKE) -C build-gcc-$(GCC_VERSION) all-gcc
	$(MAKE) -C build-gcc-$(GCC_VERSION) all-target-libgcc
	$(MAKE) -C build-gcc-$(GCC_VERSION) install-gcc
	$(MAKE) -C build-gcc-$(GCC_VERSION) install-target-libgcc

$(TOOLCHAIN_SYSROOT)/$(TARGET)/lib/libc.a: newlib-$(NEWLIB_VERSION) $(TOOLCHAIN_PREFIX)/bin/$(TARGET)-ar $(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc
	mkdir -pv build-newlib-$(NEWLIB_VERSION)
	export PATH="$(TOOLCHAIN_PREFIX)/bin:$$PATH"; cd build-newlib-$(NEWLIB_VERSION) && ../newlib-$(NEWLIB_VERSION)/configure --target=$(TARGET) --prefix=/
	export PATH="$(TOOLCHAIN_PREFIX)/bin:$$PATH"; $(MAKE) -C build-newlib-$(NEWLIB_VERSION) all
	export PATH="$(TOOLCHAIN_PREFIX)/bin:$$PATH"; $(MAKE) -C build-newlib-$(NEWLIB_VERSION) DESTDIR=$(TOOLCHAIN_SYSROOT) install

.PHONY: clean
clean:
	rm -rfv "$(TOOLCHAIN_PREFIX)"
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION) clean
	$(MAKE) -C build-gcc-$(GCC_VERSION) clean
	$(MAKE) -C build-newlib-$(NEWLIB_VERSION) clean

.PHONY: distclean
distclean:
	rm -rfv "$(TOOLCHAIN_PREFIX)"
	rm -rfv "build-nasm-$(NASM_VERSION)"
	rm -rfv "build-binutils-$(BINUTILS_VERSION)"
	rm -rfv "build-gcc-$(GCC_VERSION)"
	rm -rfv "build-newlib-$(NEWLIB_VERSION)"
