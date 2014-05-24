[GLOBAL gdt_flush]
[BITS 32]

gdt_flush:
; flush gdt
; parameters:
;     gdt pointer

mov eax,[esp+4]
lgdt [eax]

mov ax,0x10
mov ds,ax
mov ss,ax
mov es,ax
mov fs,ax
mov gs,ax

; jump to new code segment
jmp 0x8:flush

flush:
ret
