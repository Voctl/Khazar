#include "../include/memory/pmm.h"
#include "../include/typint.h"
#include "../include/vga.h"
#include "../kernel/panic.h"
#include "../include/memory/pml.h"



static volatile uint32_t *frames;
static size_t nframes;
static size_t total_memory = 0;
static size_t unavailable_mem = 0;

#define PSHIFT 12
#define PSIZE 0x1000UL
#define PSIZE_MASK 0xFFFFffffFFFFf000UL
#define PLOW_MASK 0x0000000000000FFFUL

#define INDEX_FROM_BIT(b)  ((b) >> 5)
#define OFFSET_FROM_BIT(b) ((b) & 0x1F)

static uintptr_t lowest_available = 0;

/* Bitmap-de verilen fiziki unvanin bitini 1 edir (dolu olaraq isaretleyir) */
void pmm_set_addr(uintptr_t addr){
    if(addr < nframes * PSIZE){
        uint64_t frame = addr >> PSHIFT;
        uint64_t index = INDEX_FROM_BIT(frame);
        uint32_t offset = OFFSET_FROM_BIT(frame);

        frames[index] |= ((uint32_t)1 << offset);
        asm("" ::: "memory");
    }
}

/* Bitmap-de verilen fiziki unvanin bitini 0 edir (bos olaraq isaretleyir) */
void pmm_clear(uintptr_t addr){
    if(addr < nframes * PSIZE){
        uint64_t frame = addr >> PSHIFT;
        uint64_t index = INDEX_FROM_BIT(frame);
        uint32_t offset = OFFSET_FROM_BIT(frame);
        
        frames[index] &= ~((uint32_t)1 << offset);
        asm("" ::: "memory");
        
        if (frame < lowest_available) lowest_available = frame;
    }
}

/* Sehifenin dolu ve ya bos oldugunu yoxlayir (1 = dolu, 0 = bos) */
int pmm_test_addr(uintptr_t addr){
    if (!(addr < nframes * PSIZE )) return 1;
    uint64_t frame = addr >> PSHIFT;
    uint64_t index = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    
    asm("" ::: "memory");
    return !!(frames[index] & ((uint32_t)1 << offset));
}

/* Bitmap-i tarayaraq tapilan ilk bos sehifenin indeksini qaytarir */
uintptr_t pmm_first_free(void) {
    uintptr_t i, j;
    for (i = INDEX_FROM_BIT(lowest_available); i < INDEX_FROM_BIT(nframes); ++i) {
        if (frames[i] != (uint32_t)-1) {
            for (j = 0; j < (sizeof(uint32_t) * 8); ++j) {
                uint32_t testFrame = (uint32_t)1 << j;
                if (!(frames[i] & testFrame)) {
                    uintptr_t out = (i << 5) + j;
                    lowest_available = out + 1;
                    return out;
                }
            }
        }
    }
    kernel_panic("Out of Memory");
    return (uintptr_t)-1;
}

/* Bos bir fiziki sehife ayirir ve onun fiziki unvanini qaytarir */
uintptr_t pmm_alloc(void){
    uintptr_t index = pmm_first_free();
    if (index != (uintptr_t)-1){
        pmm_set_addr(index << PSHIFT);
        return index << PSHIFT;
    }
    return 0;
}

extern uint8_t p4_table[];

/* Kernelin ilkin sehife kataloqunun base unvanini qaytarir */
union PML * mmu_get_kernel_directory(void) {
    return (union PML*)&p4_table[0];
}

/* Verilen PML4 kataloqu altindaki butun sehife strukturlarini temizleyir */
void mmu_free(union PML * from) {
    if (!from) return;

    /* PML4 (Page Map Level 4) seviyyesini gezirik */
    for (size_t i = 0; i < 512; ++i) {
        if (from[i].bits.present) {
            union PML * pdp_in = (union PML*)((uintptr_t)from[i].bits.page << PSHIFT);
            
            /* PDPT (Page Directory Pointer Table) seviyyesini gezirik */
            for (size_t j = 0; j < 512; ++j) {
                if (pdp_in[j].bits.present) {
                    union PML * pd_in = (union PML*)((uintptr_t)pdp_in[j].bits.page << PSHIFT);
                    
                    /* PD (Page Directory) seviyyesini gezirik */
                    for (size_t k = 0; k < 512; ++k) {
                        if (pd_in[k].bits.present) {
                            union PML * pt_in = (union PML*)((uintptr_t)pd_in[k].bits.page << PSHIFT);
                            
                            /* PT (Page Table) seviyyesini gezirik - esil fiziki sehifeler */
                            for (size_t l = 0; l < 512; ++l) {
                                if (pt_in[l].bits.present) {
                                    pmm_clear((uintptr_t)pt_in[l].bits.page << PSHIFT);
                                }
                            }
                            /* PT sehife cedvelinin oz frame-ini temizleyirik */
                            pmm_clear((uintptr_t)pd_in[k].bits.page << PSHIFT);
                        }
                    }
                    /* PD kataloqunun oz frame-ini temizleyirik */
                    pmm_clear((uintptr_t)pdp_in[j].bits.page << PSHIFT);
                }
            }
            /* PDPT strukturunun oz frame-ini temizleyirik */
            pmm_clear((uintptr_t)from[i].bits.page << PSHIFT);
        }
    }
    /* En basdaki PML4 strukturunun ozunu de yaddasda bosa cixaririq */
    pmm_clear((uintptr_t)from);
}
