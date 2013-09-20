[BITS 16]
[ORG 0x7C00]

reset_drive:

; command: reset drive
mov ah,0

; interrupt: hard disk
int 0x13
	
; if error not zero, retry
or ah,ah
jnz reset_drive

; command: print character
mov ah,0x0E

mov al,'A'

; white foreground, black background, not blinking
mov bh,0x0E

; page 0
xor bl,bl

; interrupt: screen
int 0x10

; loop
jmp $

; fill with zeros
times 510-($-$$) db 0

; boot sector signature
db 0x55
db 0xAA
