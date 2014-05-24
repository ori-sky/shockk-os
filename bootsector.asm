[BITS 16]
[ORG 0x7C00]

; enforce cs:ip
jmp 0x0000:start
start:

reset_drive:

; command: reset drive
xor ah,ah

; interrupt: disk services
int 0x13

; cf set on error
jc reset_drive

; buffer address pointer (es:bx)
; load kernel into 0x2000
xor ax,ax
mov es,ax
mov bx,0x2000

; command: read sectors from drive
mov ah,0x2

; sectors to read
mov al,0x27

; cylinder
xor ch,ch

; sector (1-based)
; [0x1]       boot sector
; [0x2]       kernel
mov cl,0x2

; head
xor dh,dh

int 0x13
jc reset_drive

; disable interrupts
cli

; lgdt is loaded at ds:gdt_desc
xor ax,ax
mov ds,ax

; load gdt
lgdt [gdt_desc]

; enable protected mode
; cr0
; [0]  protected mode
; [1]  monitor coprocessor
; [2]  FPU emulation
; [3]  task switched
; [4]  math coprocessor extension type
; [5]  floating point error reporting
; [16] write protect
; [18] alignment mask
; [29] write-back mode
; [30] cache disable
mov eax,cr0
or eax,1
mov cr0,eax

; enter 32-bit mode
; make far jump to clear 16-bit instructions from pipeline
; [0x0]  null segment selector
; [0x8]  code segment selector
; [0x10] data segment selector
jmp 0x8:clear_pipe

[BITS 32]
clear_pipe:

; set segment registers to data segment selector
; segment registers
; [cs] code
; [ds] data
; [ss] stack
; [es] extra data
; [fs] extra data #2
; [gs] extra data #3
mov ax,0x10
mov ds,ax
mov ss,ax
mov es,ax
mov fs,ax
mov gs,ax

; table of first MB
; [    0-  3FF]   RAM         real mode, interrupt vector table
; [  400-  4FF]   RAM         BIOS data area
; [  500-9FFFF]   RAM         free memory, 7C00 used for boot sector
; [A0000-BFFFF]   video RAM   video memory
; [C0000-C7FFF]   video ROM   video BIOS
; [C8000-EFFFF]               BIOS shadow area
; [F0000-FFFFF]   ROM         system BIOS

; set stack pointer to 0x90000 - size of 0xFFFF
mov esp,0x90000

; frame buffer located at 0xB8000
; color text mode, applies to CGA, EGA, VGA

;mov byte[0xB8000],'A'
; [0-3]   foreground color
; [4-6]   background color
; [7]     blinking flag
; using:
;         0xB bright cyan foreground
;         0x1 blue background
;         0x0 not blinking
;mov byte[0xB8001],0b00011011

; enable interrupts
;sti

; jump to kernel
jmp 0x8:0x2000

; global descriptor table
gdt:

; null segment
gdt_null:
dq 0

; code segment
gdt_code:
; [0-15]  segment limiter bits 0-15
dw 0xFFFF
; [16-31] base address bits 0-15
dw 0
; [0-7]   base address bits 16-23
db 0
; [8-11]  segment type and attributes
;         [8]  CPU access flag
;         [9]  segment readable flag
;         [10] allows less privileged code segments to jump to or call this segment
;         [11] code segment flag
;         [12] code or data segment flag
; [13-14] privilege level - 0 most privileged, 3 least privileged
; [15]    segment present flag
db 0b10011010
; [16-19] segment limiter bits 16-19
; [20-22] attributes
;         [20] 'Available to System Programmers' flag - ignored by CPU
;         [21] reserved
;         [22] size flag - protected mode 32-bit and not 16-bit
; [23]    granularity - multiplies segment limit by 4kB
db 0b11001111
; [24-31] base address bits 24-31
db 0

; data segment
gdt_data:
dw 0xFFFF
dw 0
db 0
; [8-11]  segment type and attributes
;         [8]  CPU access flag
;         [9]  segment writable flag
;         [10] expand up flag - expands down if 0
;         [11] code segment flag
;         [12] code or data segment flag
; [13-14] privilege level - 0 most privileged, 3 least privileged
; [15]    segment present flag
db 0b10010010
db 0b11001111
db 0

gdt_end:

gdt_desc:
; [0-15]  gdt size in bytes
dw gdt_end - gdt
; [16-47] gdt memory address
dd gdt

; zero rest of sector
times 510-($-$$) db 0

; boot sector signature
db 0x55
db 0xAA
