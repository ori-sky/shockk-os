enable_language(ASM_NASM)

add_custom_target(bootsector
	COMMAND ${CMAKE_ASM_NASM_COMPILER} -f bin ${CMAKE_CURRENT_SOURCE_DIR}/bootsector.asm -o ${CMAKE_BINARY_DIR}/bootsector.bin
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/bootsector.asm
)
