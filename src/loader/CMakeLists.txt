add_executable(loader.o EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/loader_entry.cpp ${CMAKE_SOURCE_DIR}/src/screen.c ${CMAKE_SOURCE_DIR}/src/ports.c)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

set(CMAKE_C_FLAGS "-target ${SHK_TARGET} -nostdlib -ffreestanding -fno-asynchronous-unwind-tables -mno-sse -I${CMAKE_SOURCE_DIR}/include")
set_target_properties(loader.o PROPERTIES
	LINK_FLAGS  "-Wl,-e,${SHK_LOADER_ENTRY},-Ttext,${SHK_LOADER_ORIGIN},--build-id=none"
)
set(CMAKE_CXX_FLAGS "-target ${SHK_TARGET} -std=c++11 -nostdlib -ffreestanding -fno-asynchronous-unwind-tables -mno-sse -I${CMAKE_SOURCE_DIR}/include")
set_target_properties(loader.o PROPERTIES
	LINK_FLAGS  "-Wl,-e,${SHK_LOADER_ENTRY},-Ttext,${SHK_LOADER_ORIGIN},--build-id=none"
)

add_custom_target(loader.flat
	COMMAND objcopy -R .note -R .comment -S -O binary ${CMAKE_CURRENT_BINARY_DIR}/loader.o ${CMAKE_CURRENT_BINARY_DIR}/loader.flat
	DEPENDS loader.o
)

add_custom_target(loader
	COMMAND dd bs=512 count=8 if=/dev/zero of=${CMAKE_BINARY_DIR}/loader.bin
	COMMAND dd conv=notrunc bs=512 if=${CMAKE_CURRENT_BINARY_DIR}/loader.flat of=${CMAKE_BINARY_DIR}/loader.bin
	DEPENDS loader.flat
)
