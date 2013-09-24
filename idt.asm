; 0x0-0x200
%rep 0x200

; [0-15]  base address bits 0-15
dw 0x2200
; [16-31] selector for interrupt code segment
;         segment descriptor privilege level must be 0
dw 0x8
; [32-39] reserved
db 0
; [40-43] gate type
;         0b0101 - 32 bit task gate
;         0b0110 - 16 bit interrupt gate
;         0b0111 - 16 bit trap gate
;         0b1110 - 32 bit interrupt gate
;         0b1111 - 32 bit trap gate
; [44]    storage segment - 0 for interrupt gates
; [45-46] descriptor privilege level
; [47]    gate present bit
db 0b10001110
; [48-63] base address bits 48-63
dw 0x0

%endrep

times 0x1000-($-$$) db 0
