#ifndef SCREEN_ELF_H
#define SCREEN_ELF_H

#define SCREEN_CASE(X)       case X:  return screen << #X
#define SCREEN_DEFAULT(X, Y) default: return screen << #X << "::<INVALID> (" << static_cast<uint32_t>(Y) << ')'

Screen & operator<<(Screen &screen, const ELFIdentClass class_) {
	switch(class_) {
		SCREEN_CASE(ELFIdentClass::None);
		SCREEN_CASE(ELFIdentClass::x32);
		SCREEN_CASE(ELFIdentClass::x64);
		SCREEN_DEFAULT(ELFIdentClass, class_);
	}
}

Screen & operator<<(Screen &screen, const ELFIdentEncoding encoding) {
	switch(encoding) {
		SCREEN_CASE(ELFIdentEncoding::None);
		SCREEN_CASE(ELFIdentEncoding::LittleEndian);
		SCREEN_CASE(ELFIdentEncoding::BigEndian);
		SCREEN_DEFAULT(ELFIdentEncoding, encoding);
	}
}

Screen & operator<<(Screen &screen, const ELFIdentVersion version) {
	switch(version) {
		SCREEN_CASE(ELFIdentVersion::None);
		SCREEN_CASE(ELFIdentVersion::ELF1);
		SCREEN_DEFAULT(ELFIdentVersion, version);
	}
}

Screen & operator<<(Screen &screen, const ELFIdentABI abi) {
	switch(abi) {
		SCREEN_CASE(ELFIdentABI::SystemV);
		SCREEN_CASE(ELFIdentABI::Linux);
		SCREEN_CASE(ELFIdentABI::SHK);
		SCREEN_DEFAULT(ELFIdentABI, abi);
	}
}

Screen & operator<<(Screen &screen, const ELFType type) {
	switch(type) {
		SCREEN_CASE(ELFType::None);
		SCREEN_CASE(ELFType::Relocatable);
		SCREEN_CASE(ELFType::Executable);
		SCREEN_CASE(ELFType::Dynamic);
		SCREEN_CASE(ELFType::Core);
		SCREEN_DEFAULT(ELFType, type);
	}
}

Screen & operator<<(Screen &screen, const ELFMachine machine) {
	switch(machine) {
		SCREEN_CASE(ELFMachine::None);
		SCREEN_CASE(ELFMachine::Intel386);
		SCREEN_DEFAULT(ELFMachine, machine);
	}
}

Screen & operator<<(Screen &screen, const ELFIdent &ident) {
	screen << '\n';
	screen << "  magic       = " << static_cast<char>(ident.magic[0])
	                             << static_cast<char>(ident.magic[1])
	                             << static_cast<char>(ident.magic[2])
	                             << static_cast<char>(ident.magic[3])
	                                                  << '\n';
	screen << "  class       = " << ident.class_      << '\n';
	screen << "  encoding    = " << ident.encoding    << '\n';
	screen << "  version     = " << ident.version     << '\n';
	screen << "  abi         = " << ident.abi         << '\n';
	screen << "  abi_version = " << ident.abi_version;
	return screen;
}

Screen & operator<<(Screen &screen, const ELFHeader &header) {
	screen << "ident     = " << header.ident     << '\n';
	screen << "type      = " << header.type      << '\n';
	screen << "machine   = " << header.machine   << '\n';
	screen << "version   = " << header.version   << '\n';
	screen << "entry_ptr = " << header.entry_ptr << '\n';
	return screen;
}

Screen & operator<<(Screen &screen, const ELFProgramHeader &ph) {
	screen << "offset    = " << ph.offset    << '\n';
	screen << "v_addr    = " << ph.v_addr    << '\n';
	screen << "p_addr    = " << ph.p_addr    << '\n';
	screen << "file_size = " << ph.file_size << '\n';
	screen << "mem_size  = " << ph.mem_size  << '\n';
	screen << "flags     = " << ph.flags     << '\n';
	screen << "align     = " << ph.align     << '\n';
	return screen;
}

#undef SCREEN_CASE
#undef SCREEN_DEFAULT

#endif
