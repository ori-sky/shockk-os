[BITS 16]
[ORG 0x7C00]

reset_drive:

; command: reset drive
xor ah,ah

; interrupt: hard disk
int 0x13
	
; if error not zero, retry
or ah,ah
jnz reset_drive

; load idt into 0x2000
; represented by 0x0000:0x2000 in real mode
xor ax,ax
mov es,ax
mov bx,0x1000

; command: read sector from disk
mov ah,0x2

; read number of idt sectors
mov al,0x8

; disk cylinder
xor ch,ch

; disk sector
; [0x1]       boot sector
; [0x2-0x9]   idt
; [0xA]       kernel
mov cl,0x2

; disk head
xor dh,dh

; interrupt: hard disk
int 0x13

or ah,ah
jnz reset_drive

; load kernel into 0x2000
xor ax,ax
mov es,ax
mov bx,0x2000

mov ah,0x2
mov al,0x3
xor ch,ch
mov cl,0xA
xor dh,dh
int 0x13

or ah,ah
jnz reset_drive

; clear interrupts
cli

; lgdt is loaded at ds:gdt_desc so ds must be set through ax
xor ax,ax
mov ds,ax

; load gdt
lgdt [gdt_desc]

; load idt
lidt [idt_desc]

; set cr0 bit 0 to enter protected mode
mov eax,cr0
or eax,1
mov cr0,eax

; make far jump to clear 16-bit instructions from pipeline
; code segment is first segment after null segment
; multiply by 8 to get segment identifier
jmp 0x8:clear_pipe

[BITS 32]
clear_pipe:

; set data segment and stack segment to data segment identifier
mov ax,0x10
mov ds,ax
mov ss,ax

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

sti

; jump to kernel
jmp 0x8:0x2000

; global descriptor table
gdt:

gdt_null:

dq 0

gdt_code:
; [0-15]  segment limiter bits 0-15
dw 0xFFFF
; [16-31] base address bits 0-15
dw 0
; [0-7]   base address bits 16-23
db 0
; [8-11] segment type and attributes
;        [8]  CPU access flag
;        [9]  segment readable flag
;        [10] allows less privileged code segments to jump to or call this segment
;        [11] code segment flag
;        [12] code or data segment flag
; [13-14] privilege level - 0 most privileged, 3 least privileged
; [15]    segment present flag
db 0b10011010
; [16-19] segment limiter bits 16-19
; [20-22] attributes
;         [20] 'Available to System Programmers' flag - ignored by CPU
;         [21] reserved
;         [22] size flag - protected mode 32-bit and not 16-bit
; [23] granularity - multiplies segment limit by 4kB
db 0b11001111
; [24-31] base address bits 24-31
db 0

gdt_data:
dw 0xFFFF
dw 0
db 0
; [8-11] segment type and attributes
;        [8]  CPU access flag
;        [9]  segment writable flag
;        [10] expand up flag - expands down if 0
;        [11] code segment flag
;        [12] code or data segment flag
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

idt_desc:
; [0-15]  idt size in bytes - min value of 0x100
;         value of 0x1000 = 0x200 interrupts
dw 0x1000
; [16-47] idt memory address
;         idt loaded into 0x1000
dd 0x1000

; fill with zeros
times 510-($-$$) db 0

; boot sector signature
db 0x55
db 0xAA
