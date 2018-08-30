[BITS 16]                                                                       ; 16 bits
[ORG 0x500]                                                                    ; bootsector loads stage1 into 0x8000
start:
  mov [drive_ref], dl                                                           ; save drive reference
  mov [inode_offset], ecx                                                       ; save inode offset
  mov word [packet_segment], 0x7400                                             ; load block at 0x7400:0x0
  mov fs, ax
  mov eax, [fs:0x28+ecx]                                                        ; eax = direct block id 0
  call load_block                                                               ; ax = block selector
  mov gs, ax
  cmp dword [gs:0x0], `\x7fELF`                                                 ; check that we actually have an ELF
  jnz panic
.a20:
  in al, 0x92
  mov bl, al
.while:
  in al, 0x64
  and al, 2
  jnz .while
  mov al, bl
  or al, 2
  out 0x92, al
.program_headers:
  xor ecx, ecx                                                                  ; counter = 0
  .loop:
    mov eax, ecx                                                                ; copy counter to eax for mul
    mul word [gs:0x2a]                                                          ; eax = counter * e_phentsize
    mov ebx, [gs:0x1c]                                                          ; ebx = e_phoff
    add eax, ebx                                                                ; eax = e_phoff + counter * e_phentsize
    cmp dword [gs:0x0+eax], 1
    jnz panic
    xor edx, edx                                                                ; inner counter = 0
    .inner_loop:
      pushad
      mov ebx, edx                                                              ; copy block index to ebx
      shl edx, 10                                                               ; offset *= block size (1024 XXX)
      add edx, [gs:0xc+eax]                                                     ; offset += p_paddr
      shr edx, 4                                                                ; selector = offset / 16
      mov ecx, [gs:0x4+eax]                                                     ; ecx = p_offset
      shr ecx, 10                                                               ; ecx /= block size (1024 XXX)
      mov eax, ebx                                                              ; copy block index to eax for call
      add eax, ecx                                                              ; eax = block index + p_offset block id
      call get_block_id                                                         ; eax = block id
      mov word [packet_segment], dx
      call load_block                                                           ; load block into destination selector
      mov al, 'B'                                                               ; 'B' for Block
      call putc
      popad
    .inner_loop_inc:
      inc edx                                                                   ; inner counter ++
      mov ebx, edx                                                              ; offset = inner counter
      shl ebx, 10                                                               ; offset *= block size (1024 XXX)
      cmp ebx, [gs:0x14+eax]                                                    ; offset < p_memsz
      jc .inner_loop
    .inner_loop_end:
    mov al, 'H'                                                                 ; 'H' for program Header
    call putc
  .loop_inc:
    inc cx                                                                      ; counter ++
    cmp cx, [gs:0x2c]                                                           ; counter < e_phnum
    jc .loop
  .loop_end:
.protect:
  mov ebx, [gs:0x18]                                                            ; save stage2 entry point to ebx
  cli                                                                           ; disable interrupts
  xor ax, ax
  mov ds, ax                                                                    ; set data segment to null selector
  lgdt [gdt_desc]                                                               ; load dummy GDT
  mov eax, cr0                                                                  ; retrieve control register 0
  or al, 1                                                                      ; set Protection Enable bit
  mov cr0, eax                                                                  ; store updated control register 0
  jmp 0x8:.protected                                                             ; far jump to enter protected selector
[BITS 32]
.protected:
  mov ax, 0x10                                                                  ; set all data segments
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov esp, 0x70000                                                              ; set stack pointer
  mov eax, 0xBAADC0DE                                                           ; SHK bootloader signature
  jmp ebx                                                                       ; jump to stage2 entry point
[BITS 16]

putc:
  pushad
  mov ah, 0xE                                                                   ; teletype output
  mov bx, 0xF                                                                   ; bl = color, bh = page number
  int 0x10                                                                      ; video services
.end:
  popad
  ret

panic:
  mov al, 'P'                                                                   ; 'P' for Panic
  call putc
hang:
  hlt
  jmp hang

get_block_size:                                                                 ; out eax = block size
  push ecx
  mov eax, 1024                                                                 ; shift left by block shift
  mov ecx, [es:0x18]
  shl eax, cl
.end:
  pop ecx
  ret

get_block_offset:                                                               ; in eax = block id
                                                                                ; out eax = block offset
  push ebx
  push edx
  mov ebx, eax
  call get_block_size
  mul ebx
.end:
  pop edx
  pop ebx
  ret

load_lba:                                                                       ; void
  pushad
  mov ah, 0x42                                                                  ; extended read sectors from drive
  mov dl, [drive_ref]                                                           ; restore drive reference
  mov si, packet                                                                ; disk address packet
  int 0x13
  jc panic                                                                      ; carry flag set on error
.end:
  popad
  ret

load_block:                                                                     ; in eax = block id
  call get_block_offset                                                         ; eax = block offset
  shr eax, 9                                                                    ; divide by LBA block size (512)
  mov dword [packet_start_block], eax                                           ; start reading at offset as LBA block
  mov word [packet_count], 2                                                    ; block is 1024 bytes (XXX) so 2 sectors
  mov ax, [packet_segment]                                                      ; return segment selector
  call load_lba
.end:
  ret

get_block_id:                                                                   ; in eax = block index
                                                                                ; out eax = block id
  push ebx
  push ecx
  push gs
  mov ecx, [inode_offset]                                                       ; restore inode offset to ecx
  cmp eax, 12                                                                   ; if block index is not direct
  jnc .singly                                                                   ; it might be singly indirect
  mov eax, [fs:0x28+ecx+eax*4]                                                  ; eax = direct block id
  jmp .end
.singly:
  sub eax, 12                                                                   ; eax = singly indirect index
  cmp eax, 256                                                                  ; if block index is not singly indirect
  jnc .doubly                                                                   ; it might be doubly indirect
  mov ebx, eax                                                                  ; copy eax to ebx
  mov word [packet_segment], 0x7500                                             ; load block at 0x7500:0x0
  mov eax, [fs:0x58+ecx]                                                        ; eax = singly indirect block id
  call load_block                                                               ; ax = singly indirect block selector
  mov gs, ax
  mov eax, [gs:0x0+ebx*4]                                                       ; eax = direct block id
  jmp .end
.doubly:
  sub eax, 256                                                                  ; eax = doubly indirect index
  cmp eax, 256*256                                                              ; if block index is not doubly indirect
  jnc .triply                                                                   ; it might be triply indirect
.triply:
  sub eax, 256*256                                                              ; eax = triply indirect index
  cmp eax, 256*256*256                                                          ; if block index is not triply indirect
  jnc panic                                                                     ; loader.elf got too big for ext2!
.end:
  pop gs
  pop ecx
  pop ebx
  ret

; vars

inode_offset:
  dq 0

drive_ref:
  db 0

packet:
  db packet_end - packet                                                        ; packet size
  db 0                                                                          ; reserved
packet_count:
  dw 0x2                                                                        ; block count
  dw 0x0                                                                        ; dst offset
packet_segment:
  dw 0x7400                                                                     ; dst segment (0x7400:0x0 == 0x74000)
packet_start_block:
  dq 0x2                                                                        ; start block
packet_end:

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
    dw gdt_end - gdt - 1                                                        ; gdt size in bytes
    dd gdt                                                                      ; gdt memory address

  times 510-($-$$) db 0                                                         ; fill rest of sector with zeroes
  db 0x44, 0xBB
