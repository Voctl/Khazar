#include "gdt.h"
#include "idt.h"
#include "typint.h"

extern void gdt_flush(U64); // this will define ins assm **

static void gdt_set_gate(U32, U32, U32, U8, U8);

gdtent_t gdt_entries[3];
gdtptr_t gdt_ptr;

void init_gdt() {
  gdt_ptr.limit = sizeof(gdt_entries) - 1;
  gdt_ptr.base = (U64)&gdt_entries;

  gdt_set_gate(0, 0, 0, 0, 0);                  // Null segment
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xAF);   // Kernel Code (ring0, 64-bit)
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);   // Kernel Data (ring0)

  gdt_flush((U64)&gdt_ptr);
}

// a function to set a gate in the GDT
static void gdt_set_gate(U32 num, U32 base, U32 limit,
                         U8 access, U8 gran) {
  gdt_entries[num].base_low = (base & 0xFFFF);
  gdt_entries[num].base_middle = (base >> 16) & 0xFF;
  gdt_entries[num].base_high = (base >> 24) & 0xFF;
  gdt_entries[num].limit_low = (limit & 0xFFFF);
  gdt_entries[num].granularity = (limit >> 16) & 0x0F;
  gdt_entries[num].granularity |= gran & 0xF0;
  gdt_entries[num].access = access;
}
