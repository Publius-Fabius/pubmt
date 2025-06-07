
#include "pubmt/binary_heap.h"
#include <sys/types.h>
#include <stdint.h>

static void pmt_bh_heapify_up(
        pmt_bh_compare_t cmp,
        pmt_bh_swap_t swap,
        void *buffer,
        const size_t elem_size,
        ssize_t index) 
{
        uint8_t *b = buffer;

        while(index > 0) {

                const ssize_t parent = (index - 1) / 2;

                void    *parent_ptr = b + parent * elem_size,
                        *index_ptr = b + index * elem_size;

                if(cmp(index_ptr, parent_ptr)) {
                        swap(index_ptr, parent_ptr);
                        index = parent;
                } else {
                        break;
                }
        }
}

static void pmt_bh_heapify_down(
        pmt_bh_compare_t cmp,
        pmt_bh_swap_t swap,
        void *buffer,
        const size_t elem_size,
        const size_t heap_size,
        ssize_t index) 
{
        uint8_t *b = buffer;

        for(;;) {

                size_t  left = 2 * index + 1,
                        right = 2 * index + 2,
                        min = index;
   
                void    *min_pointer = min * elem_size,
                        *index_pointer = min_pointer;

                if(left < heap_size) {
                        void *left_pointer = left * elem_size;
                        if(cmp(left_pointer, min_pointer)) {
                                min = left;
                                min_pointer = left_pointer;
                                goto BRANCH_A;
                        }
                } 

                if(right < heap_size) {
                        void *right_pointer = right * elem_size;
                        if(cmp(right_pointer, min_pointer)) {
                                min = right;
                                min_pointer = right_pointer;
                                goto BRANCH_B;
                        } 
                }
                
                break;

                BRANCH_A:

                if(right < heap_size) {
                        void *right_pointer = right * elem_size;
                        if(cmp(right_pointer, min_pointer)) {
                                min = right;
                                min_pointer = right_pointer;
                        }
                }

                BRANCH_B:

                swap(index_pointer, min_pointer);

                index = min;
        }
}

void *pmt_bh_insert(pmt_bh_iface *iface, void *array, void *element)
{

}

bool pmt_bh_pop(pmt_bh_iface iface, void *heap, void *element)
{

}

void *pmt_bh_peek(pmt_bh_iface iface, void *heap)
{
        
}
