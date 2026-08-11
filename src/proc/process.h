#ifndef PROCESS_H_
#define PROCESS_H_

#include "types.h"

#define PROCS_MAX 8 // max 8 process can run for now
#define STACK_SIZE 8192 // 8kb stack for each processss

#define PROC_RUNNABLE 1 // runnable process da
#define PROC_EXITED 2
#define PROC_UNUSED 0 // empty slot

struct proc {
    int pid;    // Proses ID
    int state;  // PROC_UNUSED, PROC_RUNNABLE
    U64 sp;                // Stack pointer
    U8 stack[STACK_SIZE];  // kernel stack
};

// switch.s
extern void switch_context(U64 *prev_sp, U64 *next_sp);

// process.c
U0 proc_init(U0);
struct proc *createp(U64 entryp);
U0 yield(U0);

#endif // PROCESS_H_
