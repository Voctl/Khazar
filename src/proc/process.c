#include "process.h"
#include "types.h"

struct proc procs[PROCS_MAX]; // 8
struct proc *currentp = 0;

U0 proc_init(U0){
    for (int i = 0; i < PROCS_MAX; i++) {
        procs[i].pid = i + 1;
        procs[i].state = PROC_UNUSED;
    }
} // classic init func, u can get this shi bro its so easy :d

U0 proc_exit(U0) {
    if (currentp) {
        currentp->state = PROC_EXITED;
    }
    yield();
    while (1);
}

struct proc *createp(U64 entryp){
    struct proc *proc = 0;
    for (int i = 0; i < PROCS_MAX; ++i){
        if (procs[i].state == PROC_UNUSED){
            proc = &procs[i];
            break;
        }
    } // in this code we just searchn and finding that unused proc shi
    if (!proc) return 0; // there is no proc shi
    U64 *sp = (U64 *)&proc->stack[STACK_SIZE]; /* from "zirve" to "
                                             * dib" ready registers*/
    *(--sp) = (U64)proc_exit; // funksiya bitəndə proc_exit çağırılacaq
    *(--sp) = entryp;
    *(--sp) = 0x202; // rflags (interrupts active)
    for (int i = 0; i < 15; i++) {
        *(--sp) = 0;
    } // 15 registers for pop (when context switching works)
    // refreshing shi
    proc->sp = (U64)sp;
    proc->state = PROC_RUNNABLE;
    return proc;
}

U0 yield(U0){
    struct proc *next = 0;
    /*In this situation i want to make
     *Round-Robin thing because i dont
     *have enough knowledge about these tasks
     *and process management systems in operation
     *systems. So i decided to make
     *a Round-Robin system here. Hm :D
     *Its just finding the next runnable process
     *after the process which is running now
     **/
    // if (bum) bam ? (else) pid = 0 bum
    I64 startpid = currentp ? currentp->pid : 0; /* yeah actually iam
                                                  * using ternary operators
                                                  * i know funny  */
    for (int i = 0; i < PROCS_MAX; ++i){
        int idx = (startpid + i) % PROCS_MAX;
        if (procs[idx].state == PROC_RUNNABLE){
            next = &procs[idx];
            break;
        }
    }
    if (!next || next == currentp) return;
    struct proc *prev = currentp;
    currentp = next;
    currentp->state = PROC_RUNNING;
    if (prev) {
        if (prev->state == PROC_RUNNING) {
            prev->state = PROC_RUNNABLE;
        }
        switch_context(&prev->sp, &next->sp);
    } else {
        U64 dummy_sp;
        switch_context(&dummy_sp, &next->sp);
    }
}
