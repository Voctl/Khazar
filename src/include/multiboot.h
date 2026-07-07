#pragma once

#include "../include/typint.h"
#include "../include/vga.h"

typedef struct {
  U32 size;
  U64 addr;
  U64 len;
  U32 type;
} __attribute__((packed)) multiboot_entry_t;

typedef struct {
  U32 flags;     // hansı sahelerin dolu olduğunu göstərir
  U32 mem_lower; // 640KB-dan aşağı RAM (KB ilə)
  U32 mem_upper; // 1MB-dan yuxarı RAM (KB ilə)
  U32 boot_device;
  U32 cmdline;
  U32 mods_count;
  U32 mods_addr;
  U32 syms[4];
  U32 mmap_length; // memory map-in ümumi ölçüsü (byte)
  U32 mmap_addr;
}

__attribute__((packed)) multiboot_info_t;
