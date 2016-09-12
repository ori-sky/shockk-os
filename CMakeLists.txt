cmake_minimum_required(VERSION 3.2)

project(shockk-os)

set(SHK_TARGET        i386-none-elf)
set(SHK_LOADER_ENTRY  loader_entry)
set(SHK_LOADER_ORIGIN 0x1000)
set(SHK_KERNEL_ENTRY  kernel_entry)

add_subdirectory(src/bootsector)
add_subdirectory(src/loader)
add_subdirectory(src/kernel)

add_custom_command(
	OUTPUT shk.img
	COMMAND cat ${CMAKE_BINARY_DIR}/bootsector.bin ${CMAKE_BINARY_DIR}/loader.bin ${CMAKE_BINARY_DIR}/kernel.elf > shk.img
	DEPENDS bootsector loader kernel
)
add_custom_target(shk ALL DEPENDS ${CMAKE_BINARY_DIR}/shk.img)
