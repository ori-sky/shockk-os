[BITS 32]

[GLOBAL idt_load]
idt_load:

mov eax,[esp+4]
lidt [eax]
ret

%define EOI_MASTER 0x20
%define EOI_SLAVE  0xA0
%define EOI_PORT   0x20

%macro INTERRUPT 1
	[GLOBAL isr%1]
	isr%1:

	; error code
	push 0
	; interrupt number
	push %1
	jmp isr_stub
%endmacro

%macro INTERRUPT_ERR 1
	[GLOBAL isr%1]
	isr%1:

	push %1
	jmp isr_stub
%endmacro

INTERRUPT 0
INTERRUPT 1
INTERRUPT 2
INTERRUPT 3
INTERRUPT 4
INTERRUPT 5
INTERRUPT 6
INTERRUPT 7
INTERRUPT_ERR 8
INTERRUPT 9
INTERRUPT_ERR 10
INTERRUPT_ERR 11
INTERRUPT_ERR 12
INTERRUPT_ERR 13
INTERRUPT_ERR 14

%assign i 15
%rep 256-15
	INTERRUPT i
	%assign i i+1
%endrep

[EXTERN idt_isr_handler]

isr_stub:

; push general purpose registers
push eax
push ecx
push edx
push ebx
push ebp
push esi
push edi

; push ds
xor eax,eax
mov ax,ds
push eax

mov ax,0x10
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax

; mov interrupt number into eax
; 8 32-bit registers pushed
; offset is 4 * 8
mov eax,[esp + (4 * 8)]

; return if spurious interrupt on master
cmp eax,0x27
je return

; jump forward if no master EOI required
cmp eax,0x1F
jle no_eoi

; send master EOI
mov dx,EOI_PORT
mov ax,EOI_MASTER
out dx,ax

; return if spurious interrupt on slave
cmp eax,0x2F
je return

; jump forward if no slave EOI required
cmp eax,0x27
jle no_eoi

; second slave EOI
mov ax,EOI_SLAVE
out dx,ax

no_eoi:

push esp
call idt_isr_handler

return:

mov esp,eax

pop ebx
mov ds,bx
mov es,bx
mov fs,bx
mov gs,bx

pop edi
pop esi
pop ebp
pop ebx
pop edx
pop ecx
pop eax

add esp,8

iret
