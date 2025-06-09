
#include "pubmt/dynamic_array.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct my_map_t {
        size_t capacity, size;
        int *buffer;

} my_map_t;

void *get_buffer(void *array)
{
        return ((my_map_t*)array)->buffer;
}

void set_buffer(void *array, void *buffer)
{
        ((my_map_t*)array)->buffer = buffer;
}

size_t get_size(void *array)
{
        return ((my_map_t*)array)->size;
}

void set_size(void *array, const size_t size)
{
        ((my_map_t*)array)->size = size;
}

size_t get_capacity(void *array)
{
        return ((my_map_t*)array)->capacity;
}

void set_capacity(void *array, const size_t capacity)
{
        ((my_map_t*)array)->capacity = capacity;
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

pmt_da_iface my_iface = {
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
};

void test_init()
{
        int buffer[8];
        my_map_t array;
        pmt_da_init(&my_iface, &array, buffer, 0, 8);
        assert(array.buffer = buffer);
        assert(array.capacity == 8);
        assert(array.size == 0);
}

void test_create_destroy()
{
        my_map_t array;
        pmt_da_create(&my_iface, &array, 8);
        assert(array.buffer);
        assert(array.capacity == 8);
        assert(array.size == 0);
        pmt_da_destroy(&my_iface, &array);
}

void test_clear()
{
        int buffer[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        my_map_t array;
        pmt_da_init(&my_iface, &array, buffer, 8, 8);

        pmt_da_clear(&my_iface, &array);
        assert(array.size == 0);
}

void test_zero_buffer()
{
        int buffer[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        my_map_t array;
        pmt_da_init(&my_iface, &array, buffer, 0, 8);

        assert(pmt_da_zero_buffer(&my_iface, &array, 0, 8));
        for(int x = 0; x < 8; ++x) { assert(buffer[0] == 0); }
        assert(!pmt_da_zero_buffer(&my_iface, &array, 1, 8));
}

void test_is_empty()
{
        int buffer[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        my_map_t array;
        pmt_da_init(&my_iface, &array, buffer, 1, 8);

        assert(!pmt_da_is_empty(&my_iface, &array));
        pmt_da_clear(&my_iface, &array);
        assert(pmt_da_is_empty(&my_iface, &array));
}

void test_at()
{
        int buffer[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        my_map_t array;
        pmt_da_init(&my_iface, &array, buffer, 8, 8);
        assert(array.buffer = buffer);
        assert(array.capacity == 8);
        assert(array.size == 8);

        for(int x = 0; x < 8; ++x) { 
                int *n = pmt_da_at(&my_iface, &array, (size_t)x);
                assert(*n == (x + 1)); 
        }

        assert(!pmt_da_at(&my_iface, &array, 8));
}

void test_resize()
{
        my_map_t array;
        (void)pmt_da_create(&my_iface, &array, 4);

        assert(pmt_da_resize(&my_iface, &array, 8));
        assert(array.capacity == 8);

        pmt_da_destroy(&my_iface, &array);
}

void test_scale_capacity()
{
        my_map_t array;
        (void)pmt_da_create(&my_iface, &array, 4);

        assert(pmt_da_scale_capacity(&my_iface, &array, 5));
        assert(array.capacity == 8);

        assert(pmt_da_scale_capacity(&my_iface, &array, 19));
        assert(array.capacity == 32);

        pmt_da_destroy(&my_iface, &array);
}

void test_push_back()
{
        my_map_t array;
        (void)pmt_da_create(&my_iface, &array, 4);

        int *b = array.buffer;

        int value = 1;
        assert(pmt_da_push_back(&my_iface, &array, &value) == b);
        assert(array.size == 1);
        value = 2;
        assert(pmt_da_push_back(&my_iface, &array, &value) == b + 1);
        assert(array.size == 2);
        value = 3;
        assert(pmt_da_push_back(&my_iface, &array, &value) == b + 2);
        assert(array.size == 3);
        value = 4;
        assert(pmt_da_push_back(&my_iface, &array, &value) == b + 3);
        assert(array.size == 4);

        for(int x = 0; x < 4; ++x) {
                assert(array.buffer[x] == x + 1);
        }

        value = 5;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        assert(array.capacity == 8);
        assert(array.size == 5);

        for(int x = 0; x < 5; ++x) {
                assert(array.buffer[x] == x + 1);
        }

        value = 6;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        assert(array.capacity == 8);
        assert(array.size == 6);

        for(int x = 0; x < 6; ++x) {
                assert(array.buffer[x] == x + 1);
        }

        pmt_da_destroy(&my_iface, &array);
}

void test_pop_back()
{
        my_map_t array;
        (void)pmt_da_create(&my_iface, &array, 4);

        int value = 1;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 2;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 3;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 4;
        assert(pmt_da_push_back(&my_iface, &array, &value));

        for(int x = 0; x < 4; ++x) {
                assert(array.buffer[x] == x + 1);
        }

        assert(pmt_da_pop_back(&my_iface, &array, (void*)&value));
        assert(value == 4);
        assert(array.size == 3);
        assert(pmt_da_pop_back(&my_iface, &array, (void*)&value));
        assert(value == 3);
        assert(array.size == 2);
        assert(pmt_da_pop_back(&my_iface, &array, (void*)&value));
        assert(value == 2);
        assert(array.size == 1);
        assert(pmt_da_pop_back(&my_iface, &array, (void*)&value));
        assert(value == 1);
        assert(array.size == 0);
        assert(!pmt_da_pop_back(&my_iface, &array, (void*)&value));

        pmt_da_destroy(&my_iface, &array);
}

void test_first_last()
{
        my_map_t array;
        (void)pmt_da_create(&my_iface, &array, 4);

        int value = 1;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 2;
        assert(pmt_da_push_back(&my_iface, &array, &value));

        for(int x = 0; x < 2; ++x) {
                assert(array.buffer[x] == x + 1);
        }

        int *b = array.buffer;

        assert(pmt_da_first(&my_iface, &array) == b);
        assert(pmt_da_last(&my_iface, &array) == b + 1);

        assert(pmt_da_pop_back(&my_iface, &array, NULL));
        assert(pmt_da_first(&my_iface, &array) == b);
        assert(pmt_da_last(&my_iface, &array) == b);

        assert(pmt_da_pop_back(&my_iface, &array, NULL));
        assert(pmt_da_first(&my_iface, &array) == NULL);
        assert(pmt_da_last(&my_iface, &array) == NULL);

        pmt_da_destroy(&my_iface, &array);
}

void test_insert_range()
{
        my_map_t array;
        (void)pmt_da_create(&my_iface, &array, 4);

        int value = 1;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 4;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        assert(array.capacity == 4);

        int elems2[2] = { 2, 3 };
        assert(pmt_da_insert_range(&my_iface, &array, 1, elems2, 2));
        assert(array.capacity == 4);
        for(int x = 0; x < 4; ++x) {
                assert(array.buffer[x] == x + 1);
        }

        int elems5[5] = { 4, 5, 6, 7, 8 };
        assert(pmt_da_insert_range(&my_iface, &array, 3, elems5, 5));
        assert(array.capacity == 16);
        for(int x = 0; x < 8; ++x) {
                assert(array.buffer[x] == x + 1);
        }
        assert(array.buffer[8] == 4);

        assert(pmt_da_insert_range(&my_iface, &array, 0, elems5, 1));
        for(int x = 1; x < 9; ++x) {
                assert(array.buffer[x] == x);
        }

        pmt_da_destroy(&my_iface, &array);
}

void test_remove_range()
{
        my_map_t array;
        (void)pmt_da_create(&my_iface, &array, 4);

        int value = 1;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 2;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 3;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 4;
        assert(pmt_da_push_back(&my_iface, &array, &value));

        int *b = array.buffer;
        for(int x = 0; x < 4; ++x) {
                assert(b[x] == x + 1);
        }

        assert(pmt_da_remove_range(&my_iface, &array, 1, 2));
        assert(array.size == 2);
        assert(b[0] == 1 && b[1] == 4);

        assert(pmt_da_remove_range(&my_iface, &array, 1, 1));
        assert(array.size == 1);
        assert(b[0] == 1);

        assert(pmt_da_remove_range(&my_iface, &array, 0, 1));
        assert(array.size == 0);

        value = 1;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 2;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 3;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 4;
        assert(pmt_da_push_back(&my_iface, &array, &value));

        assert(pmt_da_remove_range(&my_iface, &array, 0, 2));
        assert(array.size == 2);
        assert(b[0] == 3 && b[1] == 4);

        value = 5;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 6;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        for(int x = 0; x < 4; ++x) {
                assert(b[x] == x + 3);
        }

        assert(pmt_da_remove_range(&my_iface, &array, 2, 2));
        assert(array.size == 2);
        assert(b[0] == 3 && b[1] == 4);
        value = 5;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        value = 6;
        assert(pmt_da_push_back(&my_iface, &array, &value));
        for(int x = 0; x < 4; ++x) {
                assert(b[x] == x + 3);
        }

        assert(pmt_da_remove_range(&my_iface, &array, 1, 1));
        assert(array.size == 3);
        assert(b[0] == 3 && b[1] == 5 && b[2] == 6);

        assert(!pmt_da_remove_range(&my_iface, &array, 1, 4));
        assert(array.size == 3);
        
        pmt_da_destroy(&my_iface, &array);
}

int main(int argc, char **args)
{
        puts("testing - dynamic_array.c");

        test_init();
        test_create_destroy();
        test_clear();
        test_zero_buffer();
        test_is_empty();
        test_at();
        test_resize();
        test_scale_capacity();
        test_push_back();
        test_pop_back();
        test_first_last();
        test_insert_range();
        test_remove_range();
}