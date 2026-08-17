#ifndef PROCESS_H_
#define PROCESS_H_

#include "types.h"

#define PROCS_MAX 16
#define STACK_SIZE 8192 // 8KB Kernel Stack

/* Process states (i toke this from :
 * https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/proc.h )
 */
typedef enum {
    PROC_UNUSED = 0,
    PROC_EMBRYO,
    PROC_READY,
    PROC_RUNNABLE = 2,
    PROC_RUNNING,
    PROC_SLEEPING,
    PROC_ZOMBIE,
    PROC_EXITED = 5
} proc_state_t;

// x86_64 Context (for switch.s file :DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD)
struct context {
    U64 r15;
    U64 r14;
    U64 r13;
    U64 r12;
    U64 r11;
    U64 r10;
    U64 r9;
    U64 r8;
    U64 rbp;
    U64 rdi;
    U64 rsi;
    U64 rdx;
    U64 rcx;
    U64 rbx;
    U64 rax;
    U64 rflags;
};

struct proc {
    I64 pid;                     // Proses ID
    proc_state_t state;          // Proses state
    U64 sp;                      // Stack Pointer (RSP)
    U8 stack[STACK_SIZE];        // Kernel Stack (8KB)

    U64 sleep_ticks;             // sleep time
    I64 exit_code;               // exit code
    char name[16];               // for debug

    struct proc *next;           // Ready Queue
};

// switch.s
extern void switch_context(U64 *prev_sp, U64 *next_sp);

// process.c funcs
U0 proc_init(U0);
struct proc *createp(U64 entryp);
U0 yield(U0);

#endif // PROCESS_H_
