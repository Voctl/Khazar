#pragma once
#include "typint.h"

struct gdt_entries {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} __attribute__((packed));
typedef struct gdt_entries gdtent_t;

struct gdt_ptr {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));
typedef struct gdt_ptr gdtptr_t;

extern gdtent_t gdt_entries[3];
extern gdtptr_t gdt_ptr;

void init_gdt();
