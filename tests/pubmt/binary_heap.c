
#include "pubmt/binary_heap.h"
#include <stdio.h>
#include <assert.h>

typedef struct my_heap {
        
        size_t size, capacity;
        int *buffer;

} my_heap;

void swap(void *elem_a, void *elem_b) 
{
        int     *a = elem_a,
                *b = elem_b,
                tmp = *a;
        *a = *b;
        *b = tmp;
}

pmt_bh_swap_t get_swap(void *heap)
{
        return swap;
}

bool less_than(void *elem_a, void *elem_b) 
{
        return *((int*)elem_a) < *((int*)elem_b);
}

pmt_bh_less_than_t get_less_than(void *heap)
{
        return less_than;
}

void *get_buffer(void *array)
{
        return ((my_heap*)array)->buffer;
}

void set_buffer(void *array, void *buffer)
{
        ((my_heap*)array)->buffer = buffer;
}

size_t get_size(void *array)
{
        return ((my_heap*)array)->size;
}

void set_size(void *array, const size_t size)
{
        ((my_heap*)array)->size = size;
}

size_t get_capacity(void *array)
{
        return ((my_heap*)array)->capacity;
}

void set_capacity(void *array, const size_t capacity)
{
        ((my_heap*)array)->capacity = capacity;
}

size_t get_element_size(void *array)
{
        return sizeof(int);
}

void *my_alloc(const size_t nbytes, void *alloc_state)
{
        return malloc(nbytes);
}

void *my_realloc(void *pointer, const size_t nbytes, void *alloc_state)
{
        return realloc(pointer, nbytes);
}

void my_free(void *pointer, void *alloc_state)
{
        free(pointer);
}

pmt_da_alloc_t get_alloc(void *array)
{
        return my_alloc;
}
pmt_da_realloc_t get_realloc(void *array)
{
        return my_realloc;
}

pmt_da_free_t get_free(void *array)
{
        return my_free;
}

void *get_alloc_state(void *array)
{
        return NULL;
}

pmt_bh_iface_t my_iface = {
        .get_swap = get_swap,
        .get_less_than = get_less_than,
        .array_iface = {
                .get_alloc = get_alloc,
                .get_realloc = get_realloc,
                .get_alloc_state = get_alloc_state,
                .get_free = get_free,
                .get_buffer = get_buffer,
                .set_buffer = set_buffer,
                .get_capacity = get_capacity,
                .set_capacity = set_capacity,
                .get_size = get_size,
                .set_size = set_size,
                .get_element_size = get_element_size
        }
};

void test_insert()
{
        my_heap heap;
        pmt_da_create(&my_iface.array_iface, &heap, 8);
        
        int *b = heap.buffer;

        int value = 9;
        assert(pmt_bh_insert(&my_iface, &heap, (void*)&value) == b);
        assert(*b == 9);

        value = 8;
        assert(pmt_bh_insert(&my_iface, &heap, (void*)&value) == b);
        assert(*b == 8);

        value = 7;
        assert(pmt_bh_insert(&my_iface, &heap, (void*)&value) == b);
        assert(*b == 7);

        value = 6;
        assert(pmt_bh_insert(&my_iface, &heap, (void*)&value) == b);
        assert(*b == 6);

        value = 5;
        assert(pmt_bh_insert(&my_iface, &heap, (void*)&value) == b);
        assert(*b == 5);

        pmt_da_destroy(&my_iface.array_iface, &heap);
}

int shuffle_cmp(const void *a, const void *b)
{
        return (rand() % 3) - 1; 
}

void shuffle(int *array, size_t n)
{
        qsort(array, n, sizeof(int), shuffle_cmp);
}

int *make_deck(int min, int max)
{
        assert(min <= max);
        const size_t size = (size_t)(max - min);
        int *deck = malloc(sizeof(int) * size);
        for(int x = min; x < size; ++x)
                deck[x] = x;
        shuffle(deck, size);
        shuffle(deck, size);
        return deck;
}

void test_peek_pop()
{
        my_heap heap;
        pmt_da_create(&my_iface.array_iface, &heap, 4);
        
        int *values = make_deck(0, 100);

        for(int x = 0; x < 100; ++x) {
                assert(pmt_bh_insert(&my_iface, &heap, values + x));
        }

        for(int x = 0; x < 100; ++x) {
                int value = -1;
                assert(*((int*)pmt_bh_peek(&my_iface, &heap)) == x);
                assert(pmt_bh_pop(&my_iface, &heap, &value));
                assert(value == x);
        }

        free(values);
        pmt_da_destroy(&my_iface.array_iface, &heap);
}

int main(int argc, char **args)
{
        puts("testing - binary_heap.c");

        test_insert();
        test_peek_pop();
}