[BITS 32]
global task_switch
task_switch:
  cli
  push ebx                                                                      ; save registers not saved by cdecl
  push esi
  push edi
  push ebp
  mov esi, [esp+(4+1)*4]                                                        ; esi = pointer to next task
  mov esp, [esi]                                                                ; esp = esp of next task
  mov eax, [esi+(4+1)*4]                                                        ; eax = cr3 of next task
  mov ebx, [esi+4]                                                              ; ebx = stack top of next task
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
