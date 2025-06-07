#ifndef PUBMT_BINARY_HEAP_H
#define PUBMT_BINARY_HEAP_H

#include "pubmt/dynamic_array.h"
#include <stdlib.h>

/** Swap the contents of the two elements. */
typedef void (*pmt_bh_swap_t)(void *element_a, void *element_b);

/** Compare element_a to element_b [ -1 = less, 0 = equals, 1 = greater ]. */
typedef int (*pmt_bh_compare_t)(void *element_a, void *element_b);

/** Binary Heap Interface */
typedef struct pmt_bh_iface {

        pmt_da_iface array_iface;

        pmt_bh_swap_t (*get_swap)(void *heap);

        pmt_bh_compare_t (*get_compare)(void *heap);

} pmt_bh_iface;

/**
 * Insert an element into the binary heap.  This function may resize the 
 * internal buffer. 
 * 
 * @returns A pointer to the pushed element is returned.  A value of 'NULL' 
 * indicates a memory allocation failure.
 */
void *pmt_bh_insert(pmt_bh_iface *iface, void *array, void *element);

/**
 * Pop the minimal/maximal element from the heap.  If 'element' is not 'NULL',
 * then it will receive the contents of the popped element.
 * 
 * @returns A value of 'false' is returned if the heap is empty.
 */
bool pmt_bh_pop(pmt_bh_iface iface, void *heap, void *element);

/** 
 * Get a pointer to the minimal/maximal element. 
 * 
 * @returns A value of 'false' is returned if the heap is empty.
 */
void *pmt_bh_peek(pmt_bh_iface iface, void *heap);

#endif