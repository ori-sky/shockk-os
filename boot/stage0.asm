[BITS 16]                                                                       ; 16 bits
[ORG 0x7C00]                                                                    ; BIOS loads bootsector at 0x7C00
  cli                                                                           ; disable interrupts
  jmp 0x0:start                                                                 ; enforce segment 0x0
start:
  xor ax, ax                                                                    ; set data and stack segments to 0x0
  mov ds, ax
  mov ss, ax
  mov sp, 0x2000                                                                ; set stack pointer to 0x2000
  sti                                                                           ; enable interrupts
  mov [drive_ref], dl                                                           ; save BIOS drive reference
  mov ah, 0x41                                                                  ; check extensions present
  mov bx, 0x55AA                                                                ; must be 0x55AA
  int 0x13                                                                      ; disk services
  jc panic                                                                      ; carry flag set on not present
  cmp bx, 0xAA55                                                                ; on success, bx set to 0xAA55
  jne panic
  and cx, 0b1                                                                   ; bit 0 is disk address packet support
  jz panic
  call load_lba
.lba:
  mov si, success_msg                                                           ; string pointer
  call print                                                                    ; print string
  mov ax, 0x7000
  mov es, ax
  cmp word [es:0x38], 0xef53                                                    ; test for ext2 superblock signature
  jnz panic
  mov si, success_msg
  call print
.ext2:
  mov eax, 2                                                                    ; ext2 root inode is 2
  call load_inode                                                               ; ax:ecx = /
  mov esi, boot_str
  call lookup_inode                                                             ; ax:ecx = /boot
  mov esi, loader_elf_str
  call lookup_inode                                                             ; ax:ecx = /boot/loader.elf
.stage1:
  mov dword [packet_start_block], 1                                             ; start reading at offset as LBA block
  mov word [packet_segment], 0x50                                               ; load block at 0x50:0x0 (0x0:0x500)
  mov word [packet_count], 1                                                    ; stage1 is one sector
  call load_lba
  mov dl, [drive_ref]                                                           ; restore drive reference
  jmp 0x0:0x500                                                                 ; jump to stage1
                                                                                ; pass ax:ecx = /boot/loader.elf
                                                                                ; pass es = superblock selector

panic:
  call print
.hang:
  hlt
  jmp .hang

print:
  lodsb                                                                         ; load next character
  or al, al                                                                     ; if null character (0)
  jz .end                                                                       ; then we're done
  mov ah, 0xE                                                                   ; teletype output
  mov bx, 0xF                                                                   ; bl = color, bh = page number
  int 0x10                                                                      ; video services
  jmp print                                                                     ; loop
.end:
  ret

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

get_inode_offset:                                                               ; in eax = inode id
                                                                                ; out eax = inode offset
  push edx
  dec eax
  xor edx, edx
  div dword [es:0x28]                                                           ; eax = group id, edx = index
  shl edx, 7                                                                    ; index * sizeof(inode)
  call load_group_desc                                                          ; ax = group desc selector
  mov fs, ax
  mov eax, [fs:0x8]                                                             ; eax = inode table addr
  call get_block_offset                                                         ; inode table addr * sizeof(block)
  add eax, edx
.end:
  pop edx
  ret

get_group_desc_offset:                                                          ; in eax = group id
                                                                                ; out eax = group desc offset
  push ebx
  shl eax, 5                                                                    ; mul group id by group desc size (32)
  mov ebx, eax                                                                  ; save group id
  mov eax, [es:0x14]                                                            ; calculate superblock offset
  inc eax
  call get_block_offset
  add eax, ebx                                                                  ; calculate group desc offset
.end:
  pop ebx
  ret

load_lba:                                                                       ; void
  pushad
  mov cx, [lba_offset]                                                          ; add LBA offset to start block
  add [packet_start_block], cx
  mov ah, 0x42                                                                  ; extended read sectors from drive
  mov dl, [drive_ref]                                                           ; restore drive reference
  mov si, packet                                                                ; disk address packet
  int 0x13
  jnc .end                                                                      ; carry flag set on error
  mov si, read_error_msg
  jmp panic
.end:
  popad
  ret

load_group_desc:                                                                ; in eax = group id
                                                                                ; out ax = group desc selector
  call get_group_desc_offset                                                    ; eax = group offset
  shr eax, 9                                                                    ; divide by LBA block size (512)
  mov dword [packet_start_block], eax                                           ; start reading at offset as LBA block
  mov word [packet_segment], 0x7100                                             ; load group desc at 0x7100:0x0
  mov word [packet_count], 1                                                    ; group desc is 32 bytes so < 1 sector
  call load_lba
  mov ax, 0x7100                                                                ; return segment selector
.end:
  ret

load_block:                                                                     ; in eax = block id
                                                                                ; out ax = block selector
  call get_block_offset                                                         ; eax = block offset
  shr eax, 9                                                                    ; divide by LBA block size (512)
  mov dword [packet_start_block], eax                                           ; start reading at offset as LBA block
  mov word [packet_segment], 0x7200                                             ; load block at 0x7200:0x0
  mov word [packet_count], 2                                                    ; block is 1024 bytes (XXX) so 2 sectors
  call load_lba
  mov ax, 0x7200                                                                ; return segment selector
.end:
  ret

load_inode:                                                                     ; in eax = inode id
                                                                                ; out ax = inode selector
                                                                                ; out ecx = selector offset
  call get_inode_offset                                                         ; eax = inode offset
  mov ecx, eax                                                                  ; save inode offset to ecx
  shr eax, 9                                                                    ; divide by LBA block size (512)
  mov dword [packet_start_block], eax                                           ; start reading at offset as LBA block
  mov word [packet_segment], 0x7300                                             ; load inode at 0x7300:0x0
  mov word [packet_count], 1                                                    ; inode is 128 bytes so < 1 sector
  call load_lba
  mov ax, 0x7300                                                                ; return segment selector
  and ecx, 511                                                                  ; return selector offset (offset % 512)
.end:
  ret

lookup_inode:                                                                   ; in ax = inode selector
                                                                                ; in ecx = selector offset
                                                                                ; in esi = name
                                                                                ; out ax = inode selector
                                                                                ; out ecx = selector offset
  mov fs, ax
  mov eax, [fs:0x28+ecx]                                                        ; eax = direct block ptr 0
  call load_dirent                                                              ; ax = selector, ecx = offset
  mov fs, ax
  mov eax, [fs:0x0+ecx]                                                         ; eax = inode id
  call load_inode
.end:
  ret

load_dirent:                                                                    ; in eax = block id
                                                                                ; in esi = name
                                                                                ; out ax = dirent selector
                                                                                ; out ecx = selector offset
  call load_block                                                               ; ax = block selector
  mov fs, ax
  xor ecx, ecx                                                                  ; ecx = loop counter
.loop:
  cmp dword [fs:0x0+ecx], 0                                                     ; skip entry if inode id is 0
  jz .loop_end
  xor edx, edx                                                                  ; edx = streq loop counter
  .streq_loop:
    lea eax, [ecx+edx]                                                          ; eax = counter + streq counter
    mov bl, [esi+edx]                                                           ; ebx = search string + streq counter
    cmp [fs:0x8+eax], bl                                                        ; compare current char of both strings
    jnz .loop_inc                                                               ; if no match, skip outer loop iteration
  .streq_inc:
    inc edx                                                                     ; streq counter ++
    cmp dl, [fs:0x6+ecx]                                                        ; streq counter < len
    jc .streq_loop                                                              ; carry flag set if streq counter < len
  .streq_loop_end:                                                              ; if we got this far, found match
    mov ax, fs                                                                  ; ax = segment selector, ecx = offset
    jmp .end
.loop_inc:
  add cx, word [fs:0x4+ecx]                                                     ; counter += dirent size
  cmp cx, 1024                                                                  ; counter < 1024 (XXX)
  jc .loop                                                                      ; carry flag set if counter < 1024 (XXX)
.loop_end:                                                                      ; if we got this far, found NO match
  mov si, no_dirent_msg
  jmp panic
.end:
  ret

; syntax is S# for Success#, F# for Failure#, D# for Debug#
success_msg db 'S', 0
read_error_msg db 'R', 0
no_dirent_msg db 'D', 0

; strings
boot_str db 'boot'
loader_elf_str db 'loader.elf'

; vars
drive_ref:
  db 0

packet:
  db packet_end - packet                                                        ; packet size
  db 0                                                                          ; reserved
packet_count:
  dw 0x2                                                                        ; block count
  dw 0x0                                                                        ; dst offset
packet_segment:
  dw 0x7000                                                                     ; dst segment (0x7000:0x0 == 0x70000)
packet_start_block:
  dq 0x2                                                                        ; start block
packet_end:

  times 506-($-$$) db 0                                                         ; fill rest of sector with zeroes

; exposed vars

lba_offset:
  dd 0

  db 0x55, 0xAA                                                                 ; boot signature
