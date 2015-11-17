global isrStub
global isrErrorCodeStub
extern isrHandler

[BITS 32]                                                                       ; 32-bit instructions
isrStub:
    push 0                                                                      ; push null error code
isrErrorCodeStub:
    pushad                                                                      ; push all general-purpose registers
    call 0x8:isrHandler                                                         ; call second-stage handler
    popad                                                                       ; pop previously pushed registers
    add esp, 4                                                                  ; clean up pushed error code
    iret                                                                        ; return from interrupt
    times 512-($-$$) db 0                                                       ; fill rest of sector with zeros
