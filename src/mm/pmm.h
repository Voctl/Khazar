#pragma once

#include "types.h"
#include "multiboot.h"

union PML;

void pmm_set_addr(uintptr_t addr);
void pmm_clear(uintptr_t addr);

int pmm_test_addr(uintptr_t addr);
uintptr_t pmm_first_free(void);

void pmm_init(multiboot2_tag_mmap_t *mmap);

uintptr_t pmm_alloc(void);

extern size_t total_memory;
extern size_t reserved_memory;
extern size_t allocated_memory;

void pmm_stats(void);
