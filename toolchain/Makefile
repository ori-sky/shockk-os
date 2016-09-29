BINUTILS_VERSION=2.27
GCC_VERSION=6.2.0

.PHONY: all
all: gcc

.PHONY: binutils
binutils: $(TOOLCHAIN_PREFIX)/bin/$(TARGET)-ar

.PHONY: gcc
gcc: $(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc

$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-ar: binutils-$(BINUTILS_VERSION)
	mkdir -pv build-binutils-$(BINUTILS_VERSION)
	cd build-binutils-$(BINUTILS_VERSION) && ../$</configure --target=$(TARGET) --prefix="$(TOOLCHAIN_PREFIX)" --with-sysroot --disable-nls --disable-werror
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION)
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION) install

binutils-$(BINUTILS_VERSION): binutils-$(BINUTILS_VERSION).tar.gz
	tar -xvf $<

binutils-$(BINUTILS_VERSION).tar.gz:
	wget ftp://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VERSION).tar.gz

$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc: gcc-$(GCC_VERSION) $(TOOLCHAIN_PREFIX)/bin/$(TARGET)-ar
	mkdir -pv build-gcc-$(GCC_VERSION)
	cd build-gcc-$(GCC_VERSION) && ../$</configure --target=$(TARGET) --prefix="$(TOOLCHAIN_PREFIX)" --disable-nls --enable-languages=c,c++ --without-headers
	$(MAKE) -C build-gcc-$(GCC_VERSION) all-gcc
	$(MAKE) -C build-gcc-$(GCC_VERSION) all-target-libgcc
	$(MAKE) -C build-gcc-$(GCC_VERSION) install-gcc
	$(MAKE) -C build-gcc-$(GCC_VERSION) install-target-libgcc

gcc-$(GCC_VERSION): gcc-$(GCC_VERSION).tar.gz
	tar xvf $<
	cd gcc-$(GCC_VERSION) && sh contrib/download_prerequisites

gcc-$(GCC_VERSION).tar.gz:
	wget ftp://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.gz

.PHONY: clean
clean:
	rm -rfv "$(TOOLCHAIN_PREFIX)"
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION) clean
	$(MAKE) -C build-gcc-$(GCC_VERSION) clean

.PHONY: distclean
distclean:
	rm -rfv "$(TOOLCHAIN_PREFIX)"
	rm -rfv "build-binutils-$(BINUTILS_VERSION)"
	rm -rfv "build-gcc-$(GCC_VERSION)"
