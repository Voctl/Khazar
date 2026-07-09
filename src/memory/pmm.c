#include "../include/memory/pmm.h"
#include "../include/memory/multiboot.h"
#include "../include/memory/pml.h"
#include "../include/string.h"
#include "../include/typint.h"
#include "../include/vga.h"
#include "../kernel/panic.h"

static U32 *frames;
static size_t nframes;
static size_t total_memory = 0;
static size_t unavailable_mem = 0;

#define PSHIFT 12
#define PSIZE 0x1000UL
#define PSIZE_MASK 0xFFFFffffFFFFf000UL
#define PLOW_MASK 0x0000000000000FFFUL

#define INDEX_FROM_BIT(b) ((b) >> 5)
#define OFFSET_FROM_BIT(b) ((b)&0x1F)

static uintptr_t lowest_available = 0;

/* Bitmap-de verilen fiziki unvanin bitini 1 edir (dolu olaraq isaretleyir) */
void pmm_set_addr(uintptr_t addr) {
  if (addr < nframes * PSIZE) {
    U64 frame = addr >> PSHIFT;
    U64 index = INDEX_FROM_BIT(frame);
    U32 offset = OFFSET_FROM_BIT(frame);

    frames[index] |= ((U32)1 << offset);
    asm("" ::: "memory");
  }
}

/* Bitmap-de verilen fiziki unvanin bitini 0 edir (bos olaraq isaretleyir) */
void pmm_clear(uintptr_t addr) {
  if (addr < nframes * PSIZE) {
    U64 frame = addr >> PSHIFT;
    U64 index = INDEX_FROM_BIT(frame);
    U32 offset = OFFSET_FROM_BIT(frame);

    if (frames[index] & ((U32)1 << offset )) {
        total_memory += PSIZE;
    }
    frames[index] &= ~((U32)1 << offset);
    asm("" ::: "memory");

    if (frame < lowest_available)
      lowest_available = frame;
  }
}

/* Sehifenin dolu ve ya bos oldugunu yoxlayir (1 = dolu, 0 = bos) */
int pmm_test_addr(uintptr_t addr) {
  if (!(addr < nframes * PSIZE))
    return 1;
  U64 frame = addr >> PSHIFT;
  U64 index = INDEX_FROM_BIT(frame);
  U32 offset = OFFSET_FROM_BIT(frame);

  asm("" ::: "memory");
  return !!(frames[index] & ((U32)1 << offset));
}

/* Bitmap-i tarayaraq tapilan ilk bos sehifenin indeksini qaytarir */
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

/* Bos bir fiziki sehife ayirir ve onun fiziki unvanini qaytarir */
uintptr_t pmm_alloc(void) {
  uintptr_t index = pmm_first_free();
  if (index != (uintptr_t)-1) {
    pmm_set_addr(index << PSHIFT);
    return index << PSHIFT;
  }
  return 0;
}

/* Fiziki sehifeni geri verir (azad edir) */
void pmm_free(uintptr_t addr) { pmm_clear(addr); }

extern U8 p4_table[];

/* Kernelin ilkin sehife kataloqunun base unvanini qaytarir */
union PML *mmu_get_kernel_directory(void) {
  return (union PML *)&p4_table[0];
}

/* Verilen PML4 kataloqu altindaki butun sehife strukturlarini temizleyir */
void mmu_free(union PML *from) {
  if (!from)
    return;

  /* PML4 (Page Map Level 4) seviyyesini gezirik */
  for (size_t i = 0; i < 512; ++i) {
    if (from[i].bits.present) {
      union PML *pdp_in = (union PML *)((uintptr_t)from[i].bits.page << PSHIFT);

      /* PDPT (Page Directory Pointer Table) seviyyesini gezirik */
      for (size_t j = 0; j < 512; ++j) {
        if (pdp_in[j].bits.present) {
          union PML *pd_in =
              (union PML *)((uintptr_t)pdp_in[j].bits.page << PSHIFT);

          /* PD (Page Directory) seviyyesini gezirik */
          for (size_t k = 0; k < 512; ++k) {
            if (pd_in[k].bits.present) {
              union PML *pt_in =
                  (union PML *)((uintptr_t)pd_in[k].bits.page << PSHIFT);

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

static U32 frame_bitmap[1024 * 1024 / 8];

void pmm_init(multiboot_info_t *mb) {
  frames = frame_bitmap;
  nframes = sizeof(frame_bitmap) * 8; /* 256K frame = 1GB */

  memset(frames, 0xFF, sizeof(frame_bitmap));

  U64 ptr = mb->mmap_addr;
  U64 end = mb->mmap_addr + mb->mmap_length;
  U32 entry_count = 0; // for debug

  while (ptr < end) {
    multiboot_entry_t *e = (multiboot_entry_t *)ptr;
    entry_count++;
    if (e->type == 1) {
      total_memory += e->len;
      for (U64 addr = e->addr; addr < e->addr + e->len; addr += 0x1000)
        pmm_clear(addr);
    }

    ptr += e->size + 4;
  }

  putstr("mmap entry count: ");
  putdec(entry_count);
  putstr("\n");

  /* Low 1MB-i her zaman reserved saxla (BIOS, VGA, kernel) */
  for (U64 addr = 0; addr < 0x100000; addr += 0x1000)
    pmm_set_addr(addr);
}

void pmm_stats(void) {
  U64 real_frames = total_memory / PSIZE;
  if (real_frames > nframes) {
    real_frames = nframes;
  }

  U64 usedc = 0;
  U64 freec = 0;

  for (U64 i = 0; i < real_frames; i++) {
    if (pmm_test_addr(i << PSHIFT)) {
      usedc++;
    } else {
      freec++;
    }
  }

  U64 free_mb = (freec * PSIZE) / (1024 * 1024);
  U64 used_mb = (usedc * PSIZE) / (1024 * 1024);
  U64 total_mb = total_memory / (1024 * 1024);

  putstr("\n---PMM Memory Stats---\n");
  putstr("Total: ");
  putdec(total_mb);
  putstr(" MB (");
  putdec(real_frames);
  putstr(" frames)\n");
  putstr("Used: ");
  putdec(used_mb);
  putstr(" MB (");
  putdec(usedc);
  putstr(" frames)\n");
  putstr("Free: ");
  putdec(free_mb);
  putstr(" MB (");
  putdec(freec);
  putstr(" frames)\n");
}
