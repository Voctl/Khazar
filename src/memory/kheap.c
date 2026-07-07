#include "../include/memory/kheap.h"
#include "../include/typint.h"
#include "memory/pmm.h"

heap_t create_heap(U64 start, U64 end, U64 max, U8 supervisor, U8 readonly){
    heap_t kheap;
    kheap.start = start;
    kheap.end = end;
    kheap.max = max;
    kheap.supervisor = supervisor;
    kheap.readonly = readonly;
    kheap.index_size = 0;

    // we defining the whole area as a whole "hole"
    header_t *holehead = (header_t*) start;
    holehead->magic = HEAP_MAGIC;
    holehead->is_hole = 1;
    holehead->size = end - start;

    // we define the footer to point back to the header
    footer_t *holefoot = (footer_t*) (start + (end - start) - sizeof(footer_t));
    holefoot->magic = HEAP_MAGIC;
    holefoot->header = holehead;

    // we adding this hole to the index massive (btw its cool metod for that heap shi broo)
    kheap.index[0] = (U64)holehead;
    kheap.index_size = 1;
    return kheap;
}
