#pragma once

#include "../typint.h"
#define KHEAP_START 0xC0000000
#define KHEAP_INTIAL_SIZE 0x100000
#define HEAP_INDEX_SIZE 0x1000
#define HEAP_MAGIC 0x12312345
#define HEAP_MIN_SIZE 0x70000

typedef struct {
  U64 magic;
  U8 is_hole;
  U64 size;
} header_t;

typedef struct {
  U64 magic;
  header_t *header;
} footer_t;

typedef struct {
  U64 index[HEAP_INDEX_SIZE];
  U64 index_size;
  U64 start;
  U64 end;
  U64 max;
  U8 supervisor;
  U8 readonly;
} heap_t;

extern heap_t *kheap;

U0 init_heap(U64 start, U64 end, U64 max, U8 supervisor, U8 readonly); // kernel initializes the heap
U0* kalloc(U64 size); // kernel allocates memory
U0 kfree(U0* ptr); // kernel frees memory
