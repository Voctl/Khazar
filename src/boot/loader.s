global loader
global p4_table
extern kernel_main
extern _bss_start
extern _bss_end

MAGIC_NUMBER      equ 0x1BADB002
FLAGS             equ (1 << 1) ; grub gives mmap here i guess
CHECKSUM          equ -(MAGIC_NUMBER + FLAGS)
KERNEL_STACK_SIZE equ 4096

section .bss
align 4096
p4_table:   resb 4096
p3_table:   resb 4096
p2_table:   resb 4096
kernel_stack: resb KERNEL_STACK_SIZE

section .rodata
gdt64:
    dq 0 ; null
.code: equ $ - gdt64
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code desc
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

section .text
bits 32
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:
    mov edi, _bss_start
    mov ecx, _bss_end
    sub ecx, edi
    xor al, al
    rep stosb

    mov esp, kernel_stack + KERNEL_STACK_SIZE

    ; save multiboot pointer!! cpuid will overwrite ebx later
    mov ebp, ebx

    call check_cpuid
    call check_long_mode
    call setup_page_tables
    call enable_paging

    lgdt [gdt64.pointer]
    jmp gdt64.code:long_mode_start

check_cpuid:
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "1"
    jmp error

check_long_mode:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .no_long_mode
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .no_long_mode
    ret
.no_long_mode:
    mov al, "2"
    jmp error

setup_page_tables:
    mov eax, p3_table
    or eax, 0b11 ; present/writable
    mov [p4_table], eax

    mov eax, p2_table
    or eax, 0b11 ; present/writable
    mov [p3_table], eax

    mov ecx, 0
.loop:
    mov eax, 0x200000 ; 2mib
    mul ecx
    or eax, 0b10000011 ; present/writable/huge
    mov [p2_table + ecx*8], eax
    inc ecx
    cmp ecx, 2048 ; 4gib total
    jne .loop
    ret

enable_paging:
    ; p4 to cr3
    mov eax, p4_table
    mov cr3, eax

    ; pae bit
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; efer msr long mode bit
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; turn on paging bit
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    ret

error:
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov byte  [0xb8008], al
    hlt

bits 64
long_mode_start:
    ; clear segments
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov rsp, kernel_stack + KERNEL_STACK_SIZE

    ; pass pointer to kernel_main (edi)
    mov rdi, rbp
    call kernel_main
    hlt

global load_idt
load_idt:
    lidt [rdi]
    ret
