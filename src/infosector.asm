    db 8                                                                        ; loader sector count
    db 72                                                                       ; kernel sector count
    times 510-($-$$) db 0                                                       ; fill rest of sector with zeroes
    db 0x44, 0xBB                                                               ; infosector signature
