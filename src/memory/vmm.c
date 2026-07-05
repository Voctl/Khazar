#include "../include/memory/vmm.h"
#include "../include/typint.h"
#include "../include/vga.h"
#include "../include/memory/pml.h"
#include "../include/memory/pmm.h"
#include "../kernel/panic.h"

#include <stddef.h>

#define PAGESIZE ((size_t) 4096)

#define PAGE_TABLE_ENTRIES ((size_t) 512)
#define KERNEL_PHYS_START ((size_t) 0xffffffff80000000)
#define MEMORY_START ((size_t) 0xffff000000000000)


uint64_t* kernel_pml4 = 0;

uint64_t* get_schedule(uint64_t *table, size_t index) {
    if ((table[index] & 1) == 0) {
        uint64_t* new_table = pmm_alloc();
        if (!new_table) {
            kernel_panic("Failed to allocate page table");
            while (1);
        }
        table[index] = (uint64_t) new_table | 0b111;
    }
    return (uint64_t*)((table[index] & 0xFFFFFFFFFFFFF000) + HDDM_OFFSET);
}
