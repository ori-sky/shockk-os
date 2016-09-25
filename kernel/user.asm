extern user_entry
global user_enter

[BITS 32]                                                                       ; 32-bit instructions
user_enter:
    mov ax, 0x23                                                                ; segment selector is 0x20 | ring 3
    mov ds, ax                                                                  ; set data segment
    mov es, ax                                                                  ; set extra data segment #1
    mov fs, ax                                                                  ; set extra data segment #2
    mov gs, ax                                                                  ; set extra data segment #3
    mov eax, [esp + 4]                                                          ; save input stack pointer
    push 0x23                                                                   ; push user data segment | ring 3
    push eax                                                                    ; push stack pointer
    pushfd                                                                      ; push eflags register
    push 0x1B                                                                   ; push user code segment | ring 3
    push user_entry                                                             ; push function address
    iret                                                                        ; enter userspace
