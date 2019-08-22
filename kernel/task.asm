[BITS 32]
global _task_switch
_task_switch:
  cli
  push ebx                                                                      ; save registers not saved by cdecl
  push esi
  push edi
  push ebp
  mov edi, [esp+(4+2)*4]                                                        ; edi = pointer to current task
  cmp edi, 0                                                                    ; only save esp if not null
  je .next
  mov [edi], esp                                                                ; save kernel esp of current task
.next:
  mov esi, [esp+(4+3)*4]                                                        ; esi = pointer to next task
  mov eax, [esi+(4+1)*4]                                                        ; eax = cr3 of next task
  mov ebx, [esi+4]                                                              ; ebx = stack top of next task
  mov edi, [esp+(4+1)*4]                                                        ; edi = pointer to TSS
  mov [edi+4], ebx                                                              ; tss.esp0 = stack top of next task
  mov esp, [esi]                                                                ; esp = esp of next task
  mov ecx, cr3                                                                  ; ecx = cr3 of current task
  cmp eax, ecx                                                                  ; skip reload if same address space
  je .done
  mov cr3, eax                                                                  ; reload cr3 of next task
.done:
  pop ebp                                                                       ; restore registers not saved by cdecl
  pop edi
  pop esi
  pop ebx
  ret                                                                           ; return into next task

global _task_fork
_task_fork:
  cli
  push ebx                                                                      ; save registers not saved by cdecl
  push esi
  push edi
  push ebp
  push 1                                                                        ; return value 1 = parent
  mov edi, [esp+(4+2)*4]                                                        ; edi = pointer to current task
  cmp edi, 0                                                                    ; only save esp if not null
  je .next
  mov [edi], esp                                                                ; save kernel esp of current task
.next:
  mov esi, [esp+(4+3)*4]                                                        ; esi = pointer to next task
  mov eax, [esi+(4+1)*4]                                                        ; eax = cr3 of next task
  mov ebx, [esi+4]                                                              ; ebx = stack top of next task
  mov edi, [esp+(4+1)*4]                                                        ; edi = pointer to TSS
  mov [edi+4], ebx                                                              ; tss.esp0 = stack top of next task
  ;mov esp, [esi]                                                                ; esp = esp of next task
  mov ecx, cr3                                                                  ; ecx = cr3 of current task
  cmp eax, ecx                                                                  ; skip reload if same address space
  je .done
  mov cr3, eax                                                                  ; reload cr3 of next task
.done:
  pop eax                                                                       ; pop return value
  pop ebp                                                                       ; restore registers not saved by cdecl
  pop edi
  pop esi
  pop ebx
  ret                                                                           ; return into next task
