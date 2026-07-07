#pragma once

#include <stdint.h>
#define KHEAP_START 0xC0000000
#define KHEAP_INTIAL_SIZE 0x100000
#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MAGIC 0x12312345
#define HEAP_MIN_SIZE 0x70000

typedef struct {
    uint64_t magic;
    uint8_t is_hole;
    uint64_t size;
} header_t;

typedef struct{
    uint64_t magic;
    header_t *header;
} footer_t;

typedef struct {
    uint64_t index[HEAP_INDEX_SIZE];
    uint64_t start;
    uint64_t end;
    uint64_t max;
    uint8_t supervisor;
    uint8_t readonly;
} heap_t;
