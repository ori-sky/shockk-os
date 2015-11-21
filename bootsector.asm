[BITS 16]                                                                       ; 16-bit instructions
[ORG 0x7C00]                                                                    ; bootsector is loaded at 0x7C00
    jmp 0x0:start                                                               ; enforce cs:ip
start:
reset_drive:
    xor ah, ah                                                                  ; command = reset drive
    int 0x13                                                                    ; interrupt = disk services
    jc reset_drive                                                              ; carry flag set on error
    mov ax, 0x1000
    mov es, ax                                                                  ; es:bx = location to load kernel into
    mov bx, 0x0
    mov ah, 0x2                                                                 ; command = read sectors from drive
    mov al, 38                                                                  ; number of sectors to read
    xor ch, ch                                                                  ; disk cylinder to read from
    mov cl, 2                                                                   ; sector to begin reading from
                                                                                ; bootsector is sector 1 (1-based)
    xor dh, dh                                                                  ; disk head to read from
    int 0x13                                                                    ; interrupt = disk services
    jc reset_drive                                                              ; carry flag set on error
    cli                                                                         ; disable interrupts
    xor ax, ax                                                                  ; null selector
    mov ds, ax                                                                  ; set data segment
    lgdt [gdt_desc]                                                             ; load global descriptor table
    mov eax, cr0                                                                ; retrieve current value of cr0
    or al, 1                                                                    ; set Protection Enable bit
    mov cr0, eax                                                                ; store new value of cr0
    jmp 0x8:protected_mode                                                      ; far jump to code selector
[BITS 32]                                                                       ; 32-bit instructions
protected_mode:
    mov ax, 0x10                                                                ; data selector is 0x10
    mov ds, ax                                                                  ; set data segment
    mov ss, ax                                                                  ; set stack segment
    mov es, ax                                                                  ; set extra data segment #1
    mov fs, ax                                                                  ; set extra data segment #2
    mov gs, ax                                                                  ; set extra data segment #3
    mov esp, 0x90000                                                            ; stack from 0x90000 to 0x9FFFF
                                                                                ; video RAM begins at 0xA0000
    call 0x8:0x10000                                                            ; far jump to kernel
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
    times 510-($-$$) db 0                                                       ; fill rest of sector with zeros
    db 0x55, 0xAA                                                               ; bootsector signature



; ; enable A20 line
; call empty_8042
; mov al,0xD1 ; command write
; out 0x64,al
; call empty_8042
; mov al,0xDF ; A20 on
; out 0x60,al
; call empty_8042
; jmp load_kernel
; 
; empty_8042:
; ; loop until keyboard command queue is empty
; 
; in al,0x64
; test al,2
; jnz empty_8042
; ret
