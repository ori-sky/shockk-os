MBALIGN  equ 1 << 0                                                             ; align to page boundaries
MEMINFO  equ 1 << 1                                                             ; provide memory map
FLAGS    equ MBALIGN | MEMINFO                                                  ; multiboot flags
MAGIC    equ 0x1BADB002                                                         ; multiboot magic number
CHECKSUM equ -(MAGIC + FLAGS)                                                   ; checksum of above

section .multiboot

align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bss

align 4
stack_bottom:
	resb 16384                                                                  ; reserve 16K stack space
stack_top:

section .text

global _start:function (_start.end - _start)
extern loader_entry

[BITS 32]                                                                       ; 32-bit instructions
_start:
    lgdt [gdt_desc]                                                             ; load global descriptor table
    jmp 0x8:.gdt_loaded                                                         ; far jump to code selector
.gdt_loaded:
    mov ax, 0x10                                                                ; segment selector is 0x10
    mov ds, ax                                                                  ; set data segment
    mov ss, ax                                                                  ; set stack segment
    mov es, ax                                                                  ; set extra data segment #1
    mov fs, ax                                                                  ; set extra data segment #2
    mov gs, ax                                                                  ; set extra data segment #3
    mov esp, 0x70000                                                            ; set stack pointer
    mov byte[0xb8000], 'S'                                                      ; write signature to video memory
    mov byte[0xb8002], 'H'                                                      ; in case anything goes wrong
    mov byte[0xb8004], 'K'                                                      ; during kernel startup
	call loader_entry
.hang:                                                                          ; infinite hlt loop
	hlt
	jmp .hang
.end:

gdt:
gdt_null:                                                                       ; null segment
    dq 0                                                                        ; null
gdt_code:                                                                       ; code segment
    dw 0xFFFF                                                                   ; bits 0-15 of segment limiter
    dw 0                                                                        ; bits 0-15 of base address
    db 0                                                                        ; bits 16-23 of base address
    db 10011010b                                                                ; access byte
                                                                                ; [ 1] segment present flag
                                                                                ; [ 2] privilege ring level
                                                                                ; [ 1] data/code flag
                                                                                ; [ 1] executable (code) flag
                                                                                ; [ 1] direction/conforming flag
                                                                                ; [ 1] read/write flag
                                                                                ; [ 1] CPU access flag
    db 11001111b                                                                ; flags nibble and limiter nibble
                                                                                ; [ 1] page granularity flag
                                                                                ; [ 1] size (32-bit) flag
                                                                                ; [ 1] reserved
                                                                                ; [ 1] available to system programmers
                                                                                ; [ 4] bits 16-19 of segment limiter
    db 0                                                                        ; bits 24-31 of base address
gdt_data:                                                                       ; data segment
    dw 0xFFFF                                                                   ; bits 0-15 of segment limiter
    dw 0                                                                        ; bits 0-15 of base address
    db 0                                                                        ; bits 16-23 of base address
    db 10010010b                                                                ; access byte
    db 11001111b                                                                ; flags nibble and limiter nibble
    db 0                                                                        ; bits 24-31 of base address
gdt_end:
gdt_desc:
    dw gdt_end - gdt                                                            ; gdt size in bytes
    dd gdt                                                                      ; gdt memory address
    times 510-($-$$) db 0                                                       ; fill rest of sector with zeroes
    db 0x55, 0xAA                                                               ; bootsector signature
