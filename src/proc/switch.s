[bits 64]
global switch_context
section .text
switch_context:
    ; save rflags and general regs
    pushfq
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    ; save current rsp
    mov [rdi], rsp
    ; load new process rsp 
    mov rsp, [rsi]
    ; restore general regs
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    popfq
    ; return to new proc entryp
    ret
