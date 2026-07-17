#pragma once

#include "typint.h"
#include "vga.h"

#define KERNEL_CS 0x08 // Kernel seqment terefi

typedef struct {
  U16 low_offset;              // handler[0:15]
  U16 sel;                     // KERNEL_CS = 0x08
  U8 ist;                      // Interrupt Stack Table (0 ol)
  U8 flags;                    //
  U16 mid_offset;              // handler[16:31]
  U32 high_offset;             // handler[32:63]
  U32 reserved;                // mütleq 0 olmalıdi
} __attribute__((packed)) idtent_t; // = 16 bayt

// bu (yuxardaki kod) artiq low levelda isitafde olunan bi kod strukturudur
// hansi ki gelen kesmeni yeni zengin ne is goreceyini basa dusmek ucun

typedef struct {
  U16 limit;
  U64 base;
} __attribute__((packed)) idtptr_t;

// kesinti aninda prosessorun veziyyetini tanimasi ucun interrupt frame
struct interrupt_frame {
  U64 ip;
  U64 cs;
  U64 flags;
  U64 sp;
  U64 ss;
};

#define IDT_ENTRIES 256 // 256 giris oldugu ucun IDT girislerini 256 edirik

// c kodunun bunu extern olaraq tanimasi ucundur
extern void load_idt(U64 idtr_ptr);
void idt_init();
