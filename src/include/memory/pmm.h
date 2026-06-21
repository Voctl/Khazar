#pragma once

#include "../typint.h"
#include "multiboot.h"

union PML;

void pmm_set_addr(uintptr_t addr);
void pmm_clear(uintptr_t addr);

int pmm_test_addr(uintptr_t addr);
uintptr_t pmm_first_free(void);

void pmm_init(multiboot_info_t *mb);

uintptr_t pmm_alloc(void);
void mmu_free(union PML *from);

void pmm_stats(void);
