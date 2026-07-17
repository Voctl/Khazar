#include "kheap.h"
#include "types.h"
#include "pmm.h"
#include "../kernel/panic.h"

heap_t *heap = NULL;
static heap_t the_kernel_heap;

/* helper funcs */
static U0 remove_from_index(U32 index_to_remove) {
    for (U32 i = index_to_remove; i < the_kernel_heap.index_size - 1; i++) {
        the_kernel_heap.index[i] = the_kernel_heap.index[i + 1];
    }
    the_kernel_heap.index_size--;
}
/* helper funcs */

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

    // we adding this hole to the index massive (btw its cool metod for that heap shit broo)
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

                the_kernel_heap.index[i] = (U64)new_header;
            } else {
                remove_from_index(i);
            }
            return (U0*)((U64)header + sizeof(header_t));
        }
    }
    return NULL;
}

/* kernel free memory */
/* yanani gorur Allah */
/* bro btw i be a contributer of nimble (package manager of nim) yeyy */
U0 kfree(U0* ptr) {
    if (ptr == NULL) kernel_panic("yo king it cant ur ptr is null");
    header_t *header = (header_t*)((U64)ptr - sizeof(header_t));
    if (header->magic != HEAP_MAGIC) kernel_panic("yo king it cant ur magic is wrong");
    if (header->is_hole) kernel_panic("yo king it cant ur block is already a hole");

    header->is_hole = 1; // know the block is free now

    footer_t *footer = (footer_t*)((U64)header + header->size - sizeof(footer_t));
    footer->magic = HEAP_MAGIC;
    footer->header = header;

    // we add the block to the index
    the_kernel_heap.index[the_kernel_heap.index_size++] = (U64)header;
}

/* NOTE NOTE NOTE
 * If you allocate memory twice and then free both,
 * they will remain adjacent as two separate holes in the heap.
 * Therefore, if you request a larger block next time,
 * kalloc will not be able to allocate it, as the holes are not contiguous.
 * However, for small allocations, this does not matter.
 */

/*
 * But its not a problem for now. In future BIG BRAIN AND DIVINE INTELLECT denis will develop
 * that kheap functions
 */
