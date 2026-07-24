#pragma once

#include "types.h"

#define MULTIBOOT2_MAGIC 0xE85250D6
#define MULTIBOOT2_BOOT_MAGIC 0x36D76289

#define MULTIBOOT2_TAG_END 0
#define MULTIBOOT2_TAG_MMAP 6
#define MULTIBOOT2_TAG_FRAMEBUFFER 8

typedef struct {
  U32 total_size;
  U32 reserved;
} __attribute__((packed)) multiboot2_info_t;

typedef struct {
  U32 type;
  U32 size;
} __attribute__((packed)) multiboot2_tag_t;

typedef struct {
  U32 type;
  U32 size;
  U32 entry_size;
  U32 entry_version;
} __attribute__((packed)) multiboot2_tag_mmap_t;

typedef struct {
  U64 addr;
  U64 len;
  U32 type;
  U32 reserved;
} __attribute__((packed)) multiboot2_mmap_entry_t;

typedef struct {
  U32 type;
  U32 size;
  U64 framebuffer_addr;
  U32 framebuffer_pitch;
  U32 framebuffer_width;
  U32 framebuffer_height;
  U8  framebuffer_bpp;
  U8  framebuffer_type;
  U8  reserved;
} __attribute__((packed)) multiboot2_tag_framebuffer_t;
