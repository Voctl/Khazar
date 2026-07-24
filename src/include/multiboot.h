#pragma once

#include "types.h"

#define MULTIBOOT_FLAG_MEM (1 << 0)
#define MULTIBOOT_FLAG_MAP (1 << 6)

typedef struct {
  U32 size;
  U64 addr;
  U64 len;
  U32 type;
} __attribute__((packed)) multiboot_entry_t;

typedef struct {
  U32 flags;
  U32 mem_lower;
  U32 mem_upper;
  U32 boot_device;
  U32 cmdline;
  U32 mods_count;
  U32 mods_addr;
  U32 syms[4];
  U32 mmap_length;
  U32 mmap_addr;
  U32 framebuffer_addr;
  U32 framebuffer_pitch;
  U32 framebuffer_width;
  U32 framebuffer_height;
  U8  framebuffer_bpp;
  U8  framebuffer_type;
  U16 reserved;
} __attribute__((packed)) multiboot_info_t;
