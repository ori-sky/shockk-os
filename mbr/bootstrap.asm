[BITS 16]
[ORG 0x3000]                                                                    ; we relocate sector to 0x3000
  cli                                                                           ; disable interrupts
start:
  xor ax, ax                                                                    ; set data and stack segments to 0x0
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, 0x2000                                                                ; set stack pointer to 0x2000
  sti                                                                           ; enable interrupts
.relocate:
  mov si, 0x7c00
  mov di, 0x3000
  mov cx, 512
  rep movsb                                                                     ; relocate sector to 0x3000
  jmp 0x0:0x3020                                                                ; jump to 0x3000 + 32B

  times 32-($-$$) db 0                                                          ; fill rest of initial 32 with zeroes

.relocated:
  mov [drive_ref], dl                                                           ; save BIOS drive reference
  mov ah, 0x41                                                                  ; check extensions present
  mov bx, 0x55AA                                                                ; must be 0x55AA
  int 0x13                                                                      ; disk services
  jc panic                                                                      ; carry flag set on not present
  cmp bx, 0xAA55                                                                ; on success, bx set to 0xAA55
  jne panic
  and cx, 0b1                                                                   ; bit 0 is disk address packet support
  jz panic
  mov ah, 0x42                                                                  ; extended read sectors from drive
  mov dl, [drive_ref]                                                           ; restore drive reference
  mov si, packet                                                                ; disk address packet
  int 0x13
  jc panic                                                                      ; carry flag set on error
.loaded:
  mov ecx, [0x3000 + 0x1be + 8]                                                 ; set LBA offset in stage0
  mov [0x7dfa], ecx
  jmp 0x0:0x7c00

; panic

panic:
  mov al, 'p'
  mov ah, 0xE                                                                   ; teletype output
  mov bx, 0xF                                                                   ; bl = color, bh = page number
  int 0x10                                                                      ; video services
.hang:
  hlt
  jmp .hang

; vars

drive_ref:
  db 0

packet:
  db packet_end - packet                                                        ; packet size
  db 0                                                                          ; reserved
packet_count:
  dw 0x1                                                                        ; block count
  dw 0x0                                                                        ; dst offset
packet_segment:
  dw 0x7c0                                                                      ; dst segment (0x7c0:0x0 == 0x7c00)
packet_start_block:
  dq 2048                                                                       ; start block
packet_end:

  times 448-($-$$) db 0                                                         ; fill rest of bootstrap with zeroes
