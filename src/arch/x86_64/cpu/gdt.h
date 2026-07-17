#pragma once
#include "types.h"

struct gdt_entries {
  U16 limit_low;
  U16 base_low;
  U8 base_middle;
  U8 access;
  U8 granularity;
  U8 base_high;
} __attribute__((packed));
typedef struct gdt_entries gdtent_t;

struct gdt_ptr {
  U16 limit;
  U64 base;
} __attribute__((packed));
typedef struct gdt_ptr gdtptr_t;

extern gdtent_t gdt_entries[3];
extern gdtptr_t gdt_ptr;

void init_gdt();
