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
%endmacro

%macro DEBUG_INTERRUPT 1
    global isr_stub_%1
    isr_stub_%1:
        push %1                                                                 ; push interrupt number
        jmp isr_stub                                                            ; jump to common ISR stub
%endmacro

%macro SYSCALL_INTERRUPT 1
    global isr_stub_%1
    isr_stub_%1:
        push edx                                                                ; push argument 3
        push ecx                                                                ; push argument 2
        push ebx                                                                ; push argument 1
        push eax                                                                ; push command code
        cld                                                                     ; clear direction flag
        call syscall_main                                                       ; call C syscall handler
        add esp, 4                                                              ; clean up pushed command code
        pop ebx                                                                 ; pop argument 1
        pop ecx                                                                 ; pop argument 2
        pop edx                                                                 ; pop argument 3
        iret                                                                    ; return from interrupt
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

%assign i 15
%rep 128-15
    INTERRUPT i
    %assign i i+1
%endrep

SYSCALL_INTERRUPT 128

%assign i 129
%rep 256-129
    INTERRUPT i
    %assign i i+1
%endrep
