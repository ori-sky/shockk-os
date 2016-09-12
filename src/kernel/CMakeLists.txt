add_executable(kernel EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/kernel_entry.c)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set(CMAKE_C_FLAGS "-target ${SHK_TARGET} -nostdlib -ffreestanding -fno-asynchronous-unwind-tables -mno-sse")
set_target_properties(kernel PROPERTIES
	LINK_FLAGS  "-Wl,-e,${SHK_KERNEL_ENTRY}"
)
