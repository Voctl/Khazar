#include "../include/memory/pmm.h"
#include "../include/multiboot.h"
#include "../include/memory/pml.h"
#include "../include/string.h"
#include "../include/typint.h"
#include "../include/vga.h"
#include "../kernel/panic.h"


#define PSHIFT 12
#define PSIZE 0x1000UL
#define PSIZE_MASK 0xFFFFffffFFFFf000UL
#define PLOW_MASK 0x0000000000000FFFUL

#define INDEX_FROM_BIT(b) ((b) >> 5)
#define OFFSET_FROM_BIT(b) ((b)&0x1F)

static U32 frame_bitmap[1024 * 1024 / 8];
static U32 *frames;
static size_t nframes;
size_t total_memory = 0;      // Usable RAM
size_t reserved_memory = 0;   // BIOS/MMIO/ACPI
size_t allocated_memory = 0;  // Allocated pages

static uintptr_t lowest_available = 0;

// Linker symbols
extern U8 _kernel_start;
extern U8 _kernel_end;
extern U8 p4_table[];

// Mark physical address as used (1)
void pmm_set_addr(uintptr_t addr) {
  if (addr < nframes * PSIZE) {
    U64 frame = addr >> PSHIFT;
    U64 index = INDEX_FROM_BIT(frame);
    U32 offset = OFFSET_FROM_BIT(frame);

    frames[index] |= ((U32)1 << offset);
    asm("" ::: "memory");
  }
}

// Mark physical address as free (0)
void pmm_clear(uintptr_t addr) {
    if (addr < nframes * PSIZE) {
        U64 frame = addr >> PSHIFT;
        U64 index = INDEX_FROM_BIT(frame);
        U32 offset = OFFSET_FROM_BIT(frame);

        frames[index] &= ~((U32)1 << offset);

        asm("" ::: "memory");

        if (frame < lowest_available)
            lowest_available = frame;
    }
}

// Check if page is used (1) or free (0)
int pmm_test_addr(uintptr_t addr) {
  if (!(addr < nframes * PSIZE))
    return 1;

  U64 frame = addr >> PSHIFT;
  U64 index = INDEX_FROM_BIT(frame);
  U32 offset = OFFSET_FROM_BIT(frame);

  asm("" ::: "memory");
  return !!(frames[index] & ((U32)1 << offset));
}

// Find index of first free page
uintptr_t pmm_first_free(void) {
  uintptr_t i, j;
  if (!frames) kernel_panic("frames NULL");
  if (!nframes) kernel_panic("nframes zero");

  for (i = INDEX_FROM_BIT(lowest_available); i < INDEX_FROM_BIT(nframes); ++i) {
    if (frames[i] != (U32)-1) {
      for (j = 0; j < (sizeof(U32) * 8); ++j) {
        U32 testFrame = (U32)1 << j;
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

// Allocate a free physical page
uintptr_t pmm_alloc(void) {
    uintptr_t index = pmm_first_free();

    if (index != (uintptr_t)-1) {
        pmm_set_addr(index << PSHIFT);
        allocated_memory += PSIZE;
        return index << PSHIFT;
    }

    return 0;
}

// Free a physical page
void pmm_free(uintptr_t addr) {
    pmm_clear(addr);

    if (allocated_memory >= PSIZE)
        allocated_memory -= PSIZE;
}

// Get kernel's base page directory
union PML *mmu_get_kernel_directory(void) {
  return (union PML *)&p4_table[0];
}

// Free all page structures under PML4
void mmu_free(union PML *from) {
  if (!from) return;

  for (size_t i = 0; i < 512; ++i) {
    if (from[i].bits.present) {
      union PML *pdp_in = (union PML *)((uintptr_t)from[i].bits.page << PSHIFT);

      for (size_t j = 0; j < 512; ++j) {
        if (pdp_in[j].bits.present) {
          union PML *pd_in =
              (union PML *)((uintptr_t)pdp_in[j].bits.page << PSHIFT);

          for (size_t k = 0; k < 512; ++k) {
            if (pd_in[k].bits.present) {
              union PML *pt_in =
                  (union PML *)((uintptr_t)pd_in[k].bits.page << PSHIFT);

              for (size_t l = 0; l < 512; ++l) {
                if (pt_in[l].bits.present) {
                  pmm_clear((uintptr_t)pt_in[l].bits.page << PSHIFT);
                }
              }
              pmm_clear((uintptr_t)pd_in[k].bits.page << PSHIFT);
            }
          }
          pmm_clear((uintptr_t)pdp_in[j].bits.page << PSHIFT);
        }
      }
      pmm_clear((uintptr_t)from[i].bits.page << PSHIFT);
    }
  }
  pmm_clear((uintptr_t)from);
}

void pmm_init(multiboot_info_t *mb) {
    frames = frame_bitmap;
    nframes = sizeof(frame_bitmap) * 8; // Set max size initially
    size_t highest_addr = 0;

    // Mark all memory as used initially
    memset(frames, 0xFF, sizeof(frame_bitmap));

    U64 ptr = mb->mmap_addr;
    U64 end_mmap = mb->mmap_addr + mb->mmap_length;

    total_memory = 0;

    // Pass 1: Find highest memory address (Just for usable memory)
    while (ptr < end_mmap) {
            multiboot_entry_t *e = (multiboot_entry_t *)ptr;
            U64 region_end = e->addr + e->len;

            // Just e->type == 1 (USABLE)
            if (e->type == 1) {
                if (region_end > highest_addr) {
                    highest_addr = region_end;
                }
                total_memory += e->len;
            }
            ptr += e->size + 4;
        }

    // Calculate real max frames
    nframes = highest_addr >> PSHIFT;
    if (nframes > (sizeof(frame_bitmap) * 8)) {
        nframes = sizeof(frame_bitmap) * 8;
    }

    // Pass 2: Mark usable memory as free
    ptr = mb->mmap_addr;
    while (ptr < end_mmap) {
        multiboot_entry_t *e = (multiboot_entry_t *)ptr;
        U64 region_end = e->addr + e->len;

        if (e->type == 1) {
            for (U64 addr = e->addr; addr < region_end; addr += PSIZE) {
                pmm_clear(addr);
            }
        }
        ptr += e->size + 4;
    }

    reserved_memory = highest_addr - total_memory;

    // Protect low 1MB
    for (U64 addr = 0; addr < 0x100000; addr += PSIZE) {
        pmm_set_addr(addr);
    }

    // Protect kernel code/data
    U64 k_start = (U64)&_kernel_start & PSIZE_MASK;
    U64 k_end   = ((U64)&_kernel_end + 0xFFF) & PSIZE_MASK;

    for (U64 addr = k_start; addr < k_end; addr += PSIZE) {
        pmm_set_addr(addr);
    }
}

void pmm_stats(void) {
    size_t used_frames = 0;

    // Count used frames within available memory
    for (size_t i = 0; i < nframes; i++) {
        if (pmm_test_addr((uintptr_t)i << PSHIFT)) {
            used_frames++;
        }
    }
    allocated_memory = used_frames * PSIZE;

    U64 total_mb     = total_memory / 1024 / 1024;
    U64 reserved_mb  = reserved_memory / 1024 / 1024;
    U64 allocated_mb = allocated_memory / 1024 / 1024;

    U64 free_mb = 0;
    if (total_memory > allocated_memory) {
        free_mb = (total_memory - allocated_memory) / 1024 / 1024;
    }

    putstr("\nPhysical Memory:\n");
    putstr("  Total:      "); putdec(total_mb);     putstr(" MB\n");
    putstr("  Reserved:   "); putdec(reserved_mb);  putstr(" MB\n");
    putstr("  Allocated:  "); putdec(allocated_mb); putstr(" MB\n");
    putstr("  Free:       "); putdec(free_mb);      putstr(" MB\n");
}
