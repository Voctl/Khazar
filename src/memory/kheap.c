#include "../include/memory/kheap.h"
#include "../include/typint.h"
#include "memory/pmm.h"

heap_t *heap = NULL;
static heap_t the_kernel_heap;

// we initialize the kernel heap (boundarie tag method)
void init_heap(U64 start, U64 end, U64 max, U8 supervisor, U8 readonly) {
    the_kernel_heap.start = start;
    the_kernel_heap.end = end;
    the_kernel_heap.max = max;
    the_kernel_heap.supervisor = supervisor;
    the_kernel_heap.readonly = readonly;
    the_kernel_heap.index_size = 0;

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
    the_kernel_heap.index[0] = (U64)holehead;
    the_kernel_heap.index_size = 1;

    // now the global pointer actually points somewhere valid
    heap = &the_kernel_heap;
}


// kernel alloc function
// allocates memory from the kernel heap
U0* kalloc(U64 size) {
    U64 total_size = size + sizeof(header_t) + sizeof(footer_t);

    for (U32 i = 0; i < the_kernel_heap.index_size; i++) {
        header_t *header = (header_t*) the_kernel_heap.index[i];

        if (header->is_hole && header->size >= total_size) {
            header->is_hole = 0;
            /* allocate the memory and return a pointer to the user
             * there will be split algorithm for block */

            //added split algorithm
            U64 remaining_size = header->size - total_size;
            if (remaining_size > sizeof(header_t) + sizeof(footer_t)) {
                // we split the block into two: the allocated part and the remaining hole
                // the allocated part is updated, and the remaining hole is added to the index
                header_t *new_header = (header_t*)((U64)header + total_size);
                new_header->magic = HEAP_MAGIC;
                new_header->is_hole = 1;
                new_header->size = remaining_size;

                footer_t *new_footer = (footer_t*)((U64)new_header + remaining_size - sizeof(footer_t));
                new_footer->magic = HEAP_MAGIC;
                new_footer->header = new_header;

               // we update the size of the current block (to the allocated size)
                header->size = total_size;

                // we update the footer of the current block (to point to the new hole)
                footer_t *header_footer = (footer_t*)((U64)header + total_size - sizeof(footer_t));
                header_footer->magic = HEAP_MAGIC;
                header_footer->header = header;
                // we adding the new hole to the index
                the_kernel_heap.index[the_kernel_heap.index_size++] = (U64)new_header;
            }
            return (U0*)((U64)header + sizeof(header_t));
        }
    }
    return NULL;
}
