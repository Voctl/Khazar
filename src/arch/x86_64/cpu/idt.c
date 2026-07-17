#include "idt.h"
#include "../port_io.h"
#include "types.h"

idtent_t idt[IDT_ENTRIES]; // 256liq bir massiv

idtptr_t idtr; // idt deyisenini ve registrini saxliyir

void idt_set_gate(U8 num, U64 base, U16 sel, U8 flags) {
  idt[num].low_offset = (U16)(base & 0xFFFF); // adressin 0-15 bitleri
  idt[num].mid_offset =
      (U16)((base >> 16) & 0xFFFF); // adressin 16-31 bitleri
  idt[num].high_offset =
      (U32)((base >> 32) & 0xFFFFFFFF); // adressin 32-63 bitleri

  idt[num].sel = sel;
  idt[num].ist = 0;
  idt[num].flags = flags;
  idt[num].reserved = 0;
}

extern void *isr_stub_table[]; // Assembly-dən gələn cədvəl


static void pic_remap() {
  byte_o(0x20, 0x11);
  byte_o(0xA0, 0x11); // bunlar baslatmalardir byte gonddererek basladir

  byte_o(0x21, 0x20);
  byte_o(0xA1, 0x28); // yeni baslangic nomreleri atayir

  byte_o(0x21, 0x04);
  byte_o(0xA1, 0x02); // slave master elaqesi

  // 8086 rejiimine kecid
  byte_o(0x21, 0x01);
  byte_o(0xA1, 0x01);

  // serbest buraxiiq
  byte_o(0x21, 0x00);
  byte_o(0xA1, 0x00);
}

void idt_init() {
  idtr.limit = (sizeof(idtent_t) * IDT_ENTRIES) - 1;
  idtr.base = (U64)&idt;
  // İlk 32 istisnanı və 16 IRQ-nu avtomatik qeydiyyata alırıq
  for (int i = 0; i < 48; i++) {
    idt_set_gate(i, (U64)isr_stub_table[i], 0x08, 0x8E);
  }
  pic_remap();
  load_idt((U64)&idtr);
}

extern void load_idt(U64 idtr_ptr);
