#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/typint.h"

extern void gdt_flush(uint64_t); // this will define ins assm **

static void gdt_set_gate(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);

gdtent_t gdt_entries[3];
gdtptr_t gdt_ptr;

void init_gdt() {
  gdt_ptr.limit = sizeof(gdt_entries) - 1;
  gdt_ptr.base = (uint64_t)&gdt_entries;

  gdt_set_gate(0, 0, 0, 0, 0);                  // Null segment
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xAF);   // Kernel Code (ring0, 64-bit)
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);   // Kernel Data (ring0)

  gdt_flush((uint64_t)&gdt_ptr);
}

// a function to set a gate in the GDT
static void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit,
                         uint8_t access, uint8_t gran) {
  gdt_entries[num].base_low = (base & 0xFFFF);
  gdt_entries[num].base_middle = (base >> 16) & 0xFF;
  gdt_entries[num].base_high = (base >> 24) & 0xFF;
  gdt_entries[num].limit_low = (limit & 0xFFFF);
  gdt_entries[num].granularity = (limit >> 16) & 0x0F;
  gdt_entries[num].granularity |= gran & 0xF0;
  gdt_entries[num].access = access;
}
