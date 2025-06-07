
#include "pubmt/dynamic_array.h"
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

void *pmt_da_init(
        pmt_da_iface *iface, 
        void *array, 
        void *buffer,
        const size_t size,
        const size_t initial_capacity)
{
        assert(array);
        assert(iface);
        assert(iface->set_size);
        assert(iface->set_capacity);
        assert(iface->set_buffer);
      
        iface->set_size(array, size);
        iface->set_capacity(array, initial_capacity);
        iface->set_buffer(array, buffer);

        return array;
}

void *pmt_da_create(
        pmt_da_iface *iface, 
        void *array, 
        const size_t initial_capacity)
{
        assert(array);
        assert(iface);
        assert(iface->get_alloc);
        assert(iface->get_alloc_state);
        assert(iface->get_element_size);
   
        pmt_da_alloc_t alloc = iface->get_alloc(array);
        void *alloc_state = iface->get_alloc_state(array);
        const size_t element_size = iface->get_element_size(array);

        void *buffer = alloc(initial_capacity * element_size, alloc_state);
        if(!buffer) {
                return NULL;
        }

        return pmt_da_init(iface, array, buffer, 0, initial_capacity);
}

void pmt_da_destroy(pmt_da_iface *iface, void *array)
{
        assert(array);
        assert(iface);
        assert(iface->get_free);
        assert(iface->get_alloc_state);
        assert(iface->get_buffer);

        pmt_da_free_t free = iface->get_free(array);
        void *alloc_state = iface->get_alloc_state(array);

        free(iface->get_buffer(array), alloc_state);
}

void pmt_da_clear(pmt_da_iface *iface, void *array)
{
        assert(array);
        assert(iface);
        assert(iface->set_size);

        iface->set_size(array, 0);
}

bool pmt_da_zero_buffer(
        pmt_da_iface *iface, 
        void *array, 
        const size_t index, 
        const size_t length)
{
        assert(array);
        assert(iface);
        assert(iface->get_buffer);
        assert(iface->get_element_size);
        assert(iface->get_capacity);

        uint8_t *bytes = iface->get_buffer(array);
        const size_t 
                elem_size = iface->get_element_size(array),
                capacity = iface->get_capacity(array);

        if((index + length) > capacity) {
                return false;
        }

        (void)memset(bytes + (index * elem_size), 0, (length * elem_size));

        return true;
}

bool pmt_da_is_empty(pmt_da_iface *iface, void *array)
{
        assert(array);
        assert(iface);
        assert(iface->get_size);
        
        return iface->get_size(array) == 0;
}

void *pmt_da_at(pmt_da_iface *iface, void *array, const size_t index)
{
        assert(array);
        assert(iface);
        assert(iface->get_size);
        assert(iface->get_element_size);
        assert(iface->get_buffer);

        const size_t size = iface->get_size(array);
        if(index >= size) {
                return NULL;
        }

        const size_t element_size = iface->get_element_size(array);
        uint8_t *bytes = iface->get_buffer(array);

        return bytes + index * element_size;
}

bool pmt_da_resize(
        pmt_da_iface *iface, 
        void *array, 
        const size_t new_cap)
{
        assert(array);
        assert(iface);
        assert(iface->get_size);
        assert(iface->set_capacity);
        assert(iface->get_element_size);
        assert(iface->get_buffer);
        assert(iface->set_buffer);
        assert(iface->get_realloc);
        assert(iface->get_alloc_state);

        if(iface->get_size(array) > new_cap) {
                return false;
        }

        const size_t elem_size = iface->get_element_size(array);
        pmt_da_realloc_t realloc = iface->get_realloc(array);
        void *st = iface->get_alloc_state(array);

        void    *buffer = iface->get_buffer(array),
                *new_buf = realloc(buffer, new_cap * elem_size, st);

        if(!new_buf) {
                return false;
        }

        iface->set_buffer(array, new_buf);
        iface->set_capacity(array, new_cap);

        return true;
}

bool pmt_da_shrink_to_fit(pmt_da_iface *iface, void *array)
{
        assert(array);
        assert(iface);
        assert(iface->get_size);
        assert(iface->get_capacity);

        const size_t    size = iface->get_size(array),
                        capacity = iface->get_capacity(array);

        assert(size <= capacity);

        if(size == capacity) {
                return true;
        }

        return pmt_da_resize(iface, array, size);
}

bool pmt_da_reserve( 
        pmt_da_iface *iface, 
        void *array,
        const size_t new_capacity)
{
        assert(array);
        assert(iface);
        assert(iface->get_capacity);

        const size_t capacity = iface->get_capacity(array);
        
        if(new_capacity <= capacity) {
                return true;
        }

        return pmt_da_resize(iface, array, new_capacity);
}

static bool pmt_da_find_scaled_capacity(
        const size_t init_cap,
        const size_t want_cap,
        size_t *new_cap)
{
        size_t last_cap = init_cap;

        for(unsigned int x = 0; x < 100; ++x) {

                const size_t next_cap = last_cap * 2;

                if(next_cap <= last_cap) {
                        return false;
                } else if(next_cap >= want_cap) {
                        *new_cap = next_cap;
                        return true;
                }

                last_cap = next_cap;
        }

        return false;
}

bool pmt_da_scale_capacity(
        pmt_da_iface *iface, 
        void *array,
        const size_t want_cap)
{
        assert(array);
        assert(iface);
        assert(iface->get_capacity);

        const size_t capacity = iface->get_capacity(array);
        if(want_cap <= capacity) {
                return false;
        }

        size_t new_cap = 0;

        if(!pmt_da_find_scaled_capacity(capacity, want_cap, &new_cap)) {
                return false;
        }
        
        assert(new_cap >= want_cap);

        return pmt_da_resize(iface, array, new_cap);
}

void *pmt_da_push_back(pmt_da_iface *iface, void *array, void *elem)
{
        assert(array);
        assert(iface);
        assert(iface->get_size);
        assert(iface->set_size);
        assert(iface->get_capacity);
        assert(iface->get_buffer);
        assert(iface->get_element_size);

        const size_t    size = iface->get_size(array),
                        elem_size = iface->get_element_size(array),
                        capacity = iface->get_capacity(array);

        assert(size <= capacity);

        if(size >= capacity) {
                if(!pmt_da_scale_capacity(iface, array, size + 1)) {
                        return NULL;
                }
        }

        uint8_t *buffer = iface->get_buffer(array),
                *pointer = buffer + size * elem_size;

        iface->set_size(array, size + 1);

        if(elem) {
                (void)memcpy(pointer, elem, elem_size);
        }

       return pointer;
}

bool pmt_da_pop_back(pmt_da_iface *iface, void *array, void *elem)
{
        assert(array);
        assert(iface);
        assert(iface->get_size);
        assert(iface->set_size);
        assert(iface->get_buffer);

        const size_t    size = iface->get_size(array),
                        last = size - 1;
        if(!size) {
                return false;
        }

        if(elem) {
                uint8_t *buffer = iface->get_buffer(array);
                const size_t elem_size = iface->get_element_size(array);
                (void)memcpy(elem, buffer + last * elem_size, elem_size);
        }
        
        iface->set_size(array, last);

        return true;
}

void *pmt_da_first(pmt_da_iface *iface, void *array)
{
        assert(array);
        assert(iface);
        assert(iface->get_size);
        assert(iface->get_buffer);
 
        const size_t size = iface->get_size(array);
        if(!size) {
                return NULL;
        }

        return iface->get_buffer(array);
}

void *pmt_da_last(pmt_da_iface *iface, void *array)
{
        assert(array);
        assert(iface);
        assert(iface->get_size);
        assert(iface->get_buffer);

        const size_t    size = iface->get_size(array),
                        elem_size = iface->get_element_size(array);

        if(!size) {
                return NULL;
        }

        uint8_t *buffer = iface->get_buffer(array);

        return buffer + (size - 1) * elem_size;
}

#include <stdio.h>

bool pmt_da_insert_range(
        pmt_da_iface *iface, 
        void *array, 
        const size_t index,
        void *elems, 
        const size_t nelems)
{
        assert(array);
        assert(iface);
        assert(iface->get_size);
        assert(iface->set_size);
        assert(iface->get_capacity);
        assert(iface->get_buffer);
        assert(iface->get_element_size);

        const size_t    size = iface->get_size(array),
                        cap = iface->get_capacity(array),
                        new_size = size + nelems;
        
        assert(index < size);

        if(new_size > cap) {
                if(!pmt_da_scale_capacity(iface, array, new_size)) {
                        return false;
                }
        }

        uint8_t *buf = iface->get_buffer(array);

        const size_t    elem_size = iface->get_element_size(array),
                        offset = index * elem_size,
                        length = nelems * elem_size,
                        chunk = (size - index) * elem_size;

        (void)memmove(buf + offset + length, buf + offset, chunk);

        (void)memcpy(buf + offset, elems, length);

        iface->set_size(array, new_size);

        return true;
}

bool pmt_da_remove_range(
        pmt_da_iface *iface, 
        void *array,
        const size_t index, 
        const size_t nelems)
{
        assert(array);
        assert(iface);
        assert(iface->get_size);
        assert(iface->set_size);
        assert(iface->get_buffer);
        assert(iface->get_element_size);

        const size_t    elem_size = iface->get_element_size(array),
                        size = iface->get_size(array),
                        offset = index + nelems;

        if(offset > size) {
                return false;
        } else if(offset == size) {
                iface->set_size(array, size - nelems);
                return true;
        }

        uint8_t *buf = iface->get_buffer(array);

        (void)memmove(
                buf + index * elem_size, 
                buf + offset * elem_size, 
                (size - offset) * elem_size);

        iface->set_size(array, size - nelems);

        return true;
}
