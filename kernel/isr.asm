extern isr_main
extern syscall_main

[BITS 32]                                                                       ; 32-bit instructions
isr_stub:
    pushad                                                                      ; push all general-purpose registers
    cld                                                                         ; clear direction flag
    call isr_main                                                               ; call second-stage handler
    popad                                                                       ; pop previously pushed registers
    add esp, 8                                                                  ; clean up interrupt number & error code
    iret                                                                        ; return from interrupt

%macro INTERRUPT 1
    global isr_stub_%1
    isr_stub_%1:
        push 0                                                                  ; push null error code
        push %1                                                                 ; push interrupt number
        jmp isr_stub                                                            ; jump to common ISR stub
        times 16-($-isr_stub_%1) db 0                                           ; align stub to 32 bytes
%endmacro

%macro DEBUG_INTERRUPT 1
    global isr_stub_%1
    isr_stub_%1:
        push %1                                                                 ; push interrupt number
        jmp isr_stub                                                            ; jump to common ISR stub
        times 16-($-isr_stub_%1) db 0                                           ; align stub to 32 bytes
%endmacro

%macro SYSCALL_INTERRUPT 1
    global isr_stub_%1
    isr_stub_%1:
        push ebp                                                                ; save ebp from ring 3
        mov ebp, esp                                                            ; save esp in ebp
        sub esp, 16                                                             ; allocate return struct on stack
        push edx                                                                ; push argument 3
        push ecx                                                                ; push argument 2
        push ebx                                                                ; push argument 1
        push eax                                                                ; push command code
        cld                                                                     ; clear direction flag
        lea eax, [ebp-8]                                                        ; calculate return struct address
        push eax                                                                ; push return struct address
        call syscall_main                                                       ; call C syscall handler
        mov eax, [ebp-8]                                                        ; retrieve first return value
        mov edx, [ebp-4]                                                        ; retrieve second return value
        leave                                                                   ; pop everything and restore ebp
        iret                                                                    ; return from interrupt
        times 32-($-isr_stub_%1) db 0                                           ; align stub to 32 bytes
%endmacro

INTERRUPT 0
INTERRUPT 1
INTERRUPT 2
INTERRUPT 3
INTERRUPT 4
INTERRUPT 5
INTERRUPT 6
INTERRUPT 7
DEBUG_INTERRUPT 8
INTERRUPT 9
DEBUG_INTERRUPT 10
DEBUG_INTERRUPT 11
DEBUG_INTERRUPT 12
DEBUG_INTERRUPT 13
DEBUG_INTERRUPT 14
INTERRUPT 15
INTERRUPT 16
INTERRUPT 17
INTERRUPT 18
INTERRUPT 19

; exceptions 20-31 are reserved by Intel and should not be used

%assign i 32
%rep 48-32
    INTERRUPT i
    %assign i i+1
%endrep

SYSCALL_INTERRUPT 128
