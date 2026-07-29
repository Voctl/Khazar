#include "pmm.h"
#include "../kernel/panic.h"
#include "multiboot.h"
#include "pml.h"
#include "string.h"
#include "types.h"
#include "vga.h"

#define PSHIFT 12
#define PSIZE 0x1000UL
#define PSIZE_MASK 0xFFFFffffFFFFf000UL
#define PLOW_MASK 0x0000000000000FFFUL

#define INDEX_FROM_BIT(b) ((b) >> 5)
#define OFFSET_FROM_BIT(b) ((b)&0x1F)

static U32 frame_bitmap[1024 * 1024 / 8];
static U32 *frames;
static size_t nframes;
size_t total_memory = 0;     // Usable RAM
size_t reserved_memory = 0;  // BIOS/MMIO/ACPI
size_t allocated_memory = 0; // Allocated pages

static uintptr_t lowest_available = 0;

// Linker symbols
extern U8 _kernel_start;
extern U8 _kernel_end;

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
  if (!frames)
    kernel_panic("frames NULL");
  if (!nframes)
    kernel_panic("nframes zero");

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

void pmm_init(multiboot2_tag_mmap_t *mmap) {
  frames = frame_bitmap;
  nframes = sizeof(frame_bitmap) * 8;
  size_t highest_addr = 0;

  memset(frames, 0xFF, sizeof(frame_bitmap));

  multiboot2_mmap_entry_t *e =
      (multiboot2_mmap_entry_t *)((U64)mmap + sizeof(multiboot2_tag_mmap_t));
  U64 end = (U64)mmap + mmap->size;

  total_memory = 0;

  while ((U64)e < end) {
    U64 region_end = e->addr + e->len;
    if (e->type == 1) {
      if (region_end > highest_addr)
        highest_addr = region_end;
      total_memory += e->len;
    }
    e = (multiboot2_mmap_entry_t *)((U64)e + mmap->entry_size);
  }

  nframes = highest_addr >> PSHIFT;
  if (nframes > (sizeof(frame_bitmap) * 8))
    nframes = sizeof(frame_bitmap) * 8;

  e = (multiboot2_mmap_entry_t *)((U64)mmap + sizeof(multiboot2_tag_mmap_t));
  while ((U64)e < end) {
    U64 region_end = e->addr + e->len;
    if (e->type == 1) {
      for (U64 addr = e->addr; addr < region_end; addr += PSIZE)
        pmm_clear(addr);
    }
    e = (multiboot2_mmap_entry_t *)((U64)e + mmap->entry_size);
  }

  reserved_memory = highest_addr - total_memory;

  for (U64 addr = 0; addr < 0x100000; addr += PSIZE)
    pmm_set_addr(addr);

  U64 k_start = (U64)&_kernel_start & PSIZE_MASK;
  U64 k_end = ((U64)&_kernel_end + 0xFFF) & PSIZE_MASK;

  for (U64 addr = k_start; addr < k_end; addr += PSIZE)
    pmm_set_addr(addr);
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

  U64 total_mb = total_memory / 1024 / 1024;
  U64 reserved_mb = reserved_memory / 1024 / 1024;
  U64 allocated_mb = allocated_memory / 1024 / 1024;

  U64 free_mb = 0;
  if (total_memory > allocated_memory) {
    free_mb = (total_memory - allocated_memory) / 1024 / 1024;
  }

  putstr((STR8_C) "\nPhysical Memory:\n");
  putstr((STR8_C) "  Total:      ");
  putdec(total_mb);
  putstr((STR8_C) " MB\n");
  putstr((STR8_C) "  Reserved:   ");
  putdec(reserved_mb);
  putstr((STR8_C) " MB\n");
  putstr((STR8_C) "  Allocated:  ");
  putdec(allocated_mb);
  putstr((STR8_C) " MB\n");
  putstr((STR8_C) "  Free:       ");
  putdec(free_mb);
  putstr((STR8_C) " MB\n");
}
