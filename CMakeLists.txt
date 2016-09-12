cmake_minimum_required(VERSION 3.2)

project(shockk-os)

set(SHK_TARGET        i386-none-elf)
set(SHK_LOADER_ENTRY  loader_entry)
set(SHK_LOADER_ORIGIN 0x1000)
set(SHK_KERNEL_ENTRY  kernel_entry)

add_subdirectory(src/bootsector)
add_subdirectory(src/loader)
add_subdirectory(src/kernel)
