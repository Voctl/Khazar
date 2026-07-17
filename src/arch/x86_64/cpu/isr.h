#ifndef ISR_H
#define ISR_H

#include "types.h"

#define IRQ0 32  // Programmable Interval Timer (TAYMER)
#define IRQ1 33  // Keyboard (KLAVIATURA)
#define IRQ2 34  // Cascade (PIC2 üçün daxili istifadə)
#define IRQ3 35  // COM2 (Serial Port 2)
#define IRQ4 36  // COM1 (Serial Port 1)
#define IRQ5 37  // LPT2 (Parallel Port 2) və ya Səs kartı
#define IRQ6 38  // Floppy Disk Controller
#define IRQ7 39  // LPT1 (Parallel Port 1)
#define IRQ8 40  // CMOS Real Time Clock
#define IRQ9 41  // ACPI (Free for peripherals)
#define IRQ10 42 // Free for peripherals (Məsələn, Şəbəkə kartı)
#define IRQ11 43 // Free for peripherals
#define IRQ12 44 // PS/2 Mouse (SIÇAN)
#define IRQ13 45 // FPU / Coprocessor Error
#define IRQ14 46 // Primary ATA Hard Disk
#define IRQ15 47 // Secondary ATA Hard Diski

typedef struct {
  U64 r15, r14, r13, r12, r11, r10, r9, r8, rbp, rdi, rsi, rdx, rcx, rbx,
      rax;
  U64 int_no, err_code;
  U64 rip, cs, rflags, rsp, ss;
} registers_t;

typedef void (*isr_t)(registers_t *);

void isr_handler(registers_t *regs);
void irq_handler(registers_t *regs);

void register_interrupt_handler(U8 n, isr_t handler);

#endif
