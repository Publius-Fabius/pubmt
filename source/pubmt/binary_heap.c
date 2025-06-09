
#include "pubmt/binary_heap.h"
#include <stdint.h>
#include <assert.h>
#include <string.h>

static void *pmt_bh_heapify_up(
        pmt_bh_less_than_t cmp,
        pmt_bh_swap_t swap,
        void *buffer,
        const size_t elem_size,
        size_t index) 
{
        uint8_t *b = buffer;

        void *index_ptr = b + index * elem_size;

        while(index > 0) {

                const size_t parent = (index - 1) / 2;
                void *parent_ptr = b + parent * elem_size;

                if(cmp(index_ptr, parent_ptr)) {
                        swap(index_ptr, parent_ptr);
                        index = parent;
                        index_ptr = parent_ptr;
                } else {
                        break;
                }
        }

        return index_ptr;
}

static void pmt_bh_heapify_down(
        pmt_bh_less_than_t cmp,
        pmt_bh_swap_t swap,
        void *buffer,
        const size_t elem_size,
        const size_t heap_size,
        size_t index) 
{
        uint8_t *b = buffer;

        void    *index_ptr = b + index * elem_size,
                *min_ptr = index_ptr;

        for(;;) {

                size_t  left = 2 * index + 1,
                        right = 2 * index + 2,
                        min = index;

                if(left < heap_size) {
                        void *left_ptr = b + left * elem_size;
                        if(cmp(left_ptr, min_ptr)) {
                                min = left;
                                min_ptr = left_ptr;
                                goto BRANCH_A;
                        }
                } 

                if(right < heap_size) {
                        void *right_ptr = b + right * elem_size;
                        if(cmp(right_ptr, min_ptr)) {
                                min = right;
                                min_ptr = right_ptr;
                                goto BRANCH_B;
                        } 
                }
                
                break;

                BRANCH_A:

                if(right < heap_size) {
                        void *right_ptr = b + right * elem_size;
                        if(cmp(right_ptr, min_ptr)) {
                                min = right;
                                min_ptr = right_ptr;
                        }
                }

                BRANCH_B:

                swap(index_ptr, min_ptr);

                index = min;
                index_ptr = min_ptr;
        }
}

void *pmt_bh_insert(pmt_bh_iface_t *iface, void *heap, void *elem)
{
        assert(heap);
        assert(elem);
        assert(iface);
        assert(iface->get_less_than);
        assert(iface->get_swap);
        assert(iface->array_iface.get_buffer);
        assert(iface->array_iface.get_size);
        assert(iface->array_iface.get_element_size);

        pmt_da_iface_t *a_iface = &iface->array_iface;
        
        const size_t index = a_iface->get_size(heap);

        if(!pmt_da_push_back(a_iface, heap, elem)) {
                return NULL;
        }
       
        return pmt_bh_heapify_up( 
                iface->get_less_than(heap),
                iface->get_swap(heap),
                a_iface->get_buffer(heap),
                a_iface->get_element_size(heap),
                index);
}

bool pmt_bh_pop(pmt_bh_iface_t *iface, void *heap, void *elem)
{
        assert(heap);
        assert(iface);
        assert(iface->get_less_than);
        assert(iface->get_swap);
        assert(iface->array_iface.get_buffer);
        assert(iface->array_iface.get_size);
        assert(iface->array_iface.get_element_size);

        pmt_da_iface_t *a_iface = &iface->array_iface;

        const size_t elem_size = a_iface->get_element_size(heap);

        void    *first = pmt_da_first(a_iface, heap),
                *last = pmt_da_last(a_iface, heap);

        if(!first) {
                return false;
        }

        if(elem) {
                (void)memcpy(elem, first, elem_size);
        }

        if(first == last) {
                pmt_da_clear(a_iface, heap);
                return true;
        }

        (void)pmt_da_pop_back(a_iface, heap, first);
        
        pmt_bh_heapify_down( 
                iface->get_less_than(heap),
                iface->get_swap(heap),
                a_iface->get_buffer(heap),
                a_iface->get_element_size(heap),
                a_iface->get_size(heap),
                0);

        return true;
}

void *pmt_bh_peek(pmt_bh_iface_t *iface, void *heap)
{
        assert(heap);
        assert(iface);
        return pmt_da_first(&iface->array_iface, heap);
}
