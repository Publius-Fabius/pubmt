
#include "pubmt/hash_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct my_node {
        
        int key;

        struct my_node_t *next;

} my_node_t;

typedef struct my_map {

        size_t capacity, size;

        void **buffer;

} my_map_t;

void *get_key(void *node)
{
      return &((my_node_t*)node)->key;
}

void set_next(void *node, void *next) 
{
        ((my_node_t*)node)->next = next;
}

void *get_next(void *node)
{
        return ((my_node_t*)node)->next;
}

void *get_buffer(void *map)
{
        return ((my_map_t*)map)->buffer;
}

void set_buffer(void *map, void *buffer)
{
        ((my_map_t*)map)->buffer = buffer;
}

size_t get_size(void *map)
{
        return ((my_map_t*)map)->size;
}

void set_size(void *map, const size_t size)
{
        ((my_map_t*)map)->size = size;
}

size_t get_capacity(void *map)
{
        return ((my_map_t*)map)->capacity;
}

void set_capacity(void *map, const size_t capacity)
{
        ((my_map_t*)map)->capacity = capacity;
}

size_t get_element_size(void *map)
{
        return sizeof(void*);
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

pmt_da_alloc_t get_alloc(void *map)
{
        return my_alloc;
}
pmt_da_realloc_t get_realloc(void *map)
{
        return my_realloc;
}

pmt_da_free_t get_free(void *map)
{
        return my_free;
}

void *get_alloc_state(void *map)
{
        return NULL;
}

bool equals(void *key_a, void *key_b)
{
        return *((int*)key_a) == *((int*)key_b);
}

size_t hash(void *ptr)
{
        return pmt_hm_fnv(ptr, sizeof(int));
}

pmt_hm_equals_t get_equals(void *map)
{
        return equals;
}

pmt_hm_hash_t get_hash(void *map)
{
        return hash;
}

pmt_hm_iface_t my_iface = {
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
                .get_element_size = get_element_size},
        .node_iface = {
                .get_next = get_next,
                .set_next = set_next
        },
        .get_key = get_key,
        .get_equals = get_equals,
        .get_hash = get_hash
};

void test_create_destroy()
{
        my_map_t map;
        pmt_hm_create(&my_iface, &map, 8);

        for(int x = 0; x < 8; ++x) {
                assert(map.buffer[x] == NULL);
        }

        pmt_hm_destroy(&my_iface, &map);
}

void test_insert_lookup()
{
        my_node_t 
                n1 = { .key = 1, .next = NULL },
                n2 = { .key = 2, .next = NULL },
                n3 = { .key = 3, .next = NULL },
                n4 = { .key = 4, .next = NULL },
                n5 = { .key = 5, .next = NULL },
                n6 = { .key = 6, .next = NULL };

        my_map_t map;
        pmt_hm_create(&my_iface, &map, 10);
        for(int x = 0; x < 10; ++x) {
                assert(map.buffer[x] == 0);
        }

        int key = 1;
        assert(pmt_hm_insert(&my_iface, &map, &n1) == PMT_HM_SUCCESS);
        assert(pmt_hm_lookup(&my_iface, &map, &key) == &n1);
        assert(pmt_hm_insert(&my_iface, &map, &n1) == PMT_HM_EXISTS);
        assert(map.size == 1);

        key = 2;
        assert(pmt_hm_insert(&my_iface, &map, &n2) == PMT_HM_SUCCESS);
        assert(pmt_hm_lookup(&my_iface, &map, &key) == &n2);
        assert(pmt_hm_insert(&my_iface, &map, &n2) == PMT_HM_EXISTS);
        assert(map.size == 2);

        key = 3;
        assert(pmt_hm_insert(&my_iface, &map, &n3) == PMT_HM_SUCCESS);
        assert(pmt_hm_lookup(&my_iface, &map, &key) == &n3);
        assert(pmt_hm_insert(&my_iface, &map, &n3) == PMT_HM_EXISTS);
        assert(map.size == 3);

        key = 4;
        assert(pmt_hm_insert(&my_iface, &map, &n4) == PMT_HM_SUCCESS);
        assert(pmt_hm_lookup(&my_iface, &map, &key) == &n4);
        assert(pmt_hm_insert(&my_iface, &map, &n4) == PMT_HM_EXISTS);
        assert(map.size == 4);

        key = 5;
        assert(pmt_hm_insert(&my_iface, &map, &n5) == PMT_HM_SUCCESS);
        assert(pmt_hm_lookup(&my_iface, &map, &key) == &n5);
        assert(pmt_hm_insert(&my_iface, &map, &n5) == PMT_HM_EXISTS);
        assert(map.size == 5);

        key = 6;
        assert(pmt_hm_insert(&my_iface, &map, &n6) == PMT_HM_SUCCESS);
        assert(pmt_hm_lookup(&my_iface, &map, &key) == &n6);
        assert(pmt_hm_insert(&my_iface, &map, &n6) == PMT_HM_EXISTS);
        assert(map.size == 6);

        key = 7;
        assert(pmt_hm_lookup(&my_iface, &map, &key) == NULL);

        pmt_hm_destroy(&my_iface, &map);
}

void test_resize()
{
        my_node_t 
                n1 = { .key = 1, .next = NULL },
                n2 = { .key = 2, .next = NULL },
                n3 = { .key = 3, .next = NULL },
                n4 = { .key = 4, .next = NULL };

        my_map_t map;
        pmt_hm_create(&my_iface, &map, 16);
        for(int x = 0; x < 16; ++x) {
                assert(map.buffer[x] == 0);
        }
        
        assert(pmt_hm_insert(&my_iface, &map, &n1) == PMT_HM_SUCCESS);
        assert(pmt_hm_insert(&my_iface, &map, &n2) == PMT_HM_SUCCESS);
        assert(pmt_hm_insert(&my_iface, &map, &n3) == PMT_HM_SUCCESS);
        assert(pmt_hm_insert(&my_iface, &map, &n4) == PMT_HM_SUCCESS);

        assert(pmt_hm_resize(&my_iface, &map, 24));
        assert(map.capacity == 24);

        int key = 1;
        assert(pmt_hm_lookup(&my_iface, &map, &key) == &n1);
        key = 2;
        assert(pmt_hm_lookup(&my_iface, &map, &key) == &n2);
        key = 3;
        assert(pmt_hm_lookup(&my_iface, &map, &key) == &n3);
        key = 4;
        assert(pmt_hm_lookup(&my_iface, &map, &key) == &n4);

        pmt_hm_destroy(&my_iface, &map);
}

void test_remove()
{
        my_node_t 
                n1 = { .key = 1, .next = NULL },
                n2 = { .key = 2, .next = NULL },
                n3 = { .key = 3, .next = NULL },
                n4 = { .key = 4, .next = NULL };

        my_map_t map;
        pmt_hm_create(&my_iface, &map, 16);
        for(int x = 0; x < 16; ++x) {
                assert(map.buffer[x] == 0);
        }
        assert(pmt_hm_insert(&my_iface, &map, &n1) == PMT_HM_SUCCESS);
        assert(pmt_hm_insert(&my_iface, &map, &n2) == PMT_HM_SUCCESS);
        assert(pmt_hm_insert(&my_iface, &map, &n3) == PMT_HM_SUCCESS);
        assert(pmt_hm_insert(&my_iface, &map, &n4) == PMT_HM_SUCCESS);

        int key = 1;
        assert(pmt_hm_remove(&my_iface, &map, &key));
        assert(!pmt_hm_lookup(&my_iface, &map, &key));
        assert(map.size == 3);

        key = 2;
        assert(pmt_hm_remove(&my_iface, &map, &key));
        assert(!pmt_hm_lookup(&my_iface, &map, &key));
        assert(map.size == 2);

        key = 3;
        assert(pmt_hm_remove(&my_iface, &map, &key));
        assert(!pmt_hm_lookup(&my_iface, &map, &key));
        assert(map.size == 1);

        pmt_hm_destroy(&my_iface, &map);
}

void test_iterator()
{
        my_map_t map;
        pmt_hm_create(&my_iface, &map, 8);
        for(int x = 0; x < 8; ++x) {
                assert(map.buffer[x] == 0);
        }

        for(int x = 0; x < 100; ++x) {
                my_node_t *node = malloc(sizeof(my_node_t));
                node->key = x;
                node->next = NULL;
                assert(pmt_hm_insert(&my_iface, &map, (void*)node) == PMT_HM_SUCCESS);
                assert(pmt_hm_insert(&my_iface, &map, (void*)node) == PMT_HM_EXISTS);
                assert(pmt_hm_lookup(&my_iface, &map, &x) == node);
        }

        pmt_hm_iter_t iter;
        pmt_hm_entries(&my_iface, &map, &iter);

        my_node_t *node;
        
        int count_table[100] = { 0 };
        int x = 0;

        while(pmt_hm_next(&my_iface, &iter, (void**)&node)) {
                count_table[x++] += 1;
                free(node);
        }

        for(int x = 0; x < 100; ++x) {
                assert(count_table[x] == 1);
        }

        pmt_hm_destroy(&my_iface, &map);
}

int main(int argc, char **args) 
{
        puts("testing - hash_map.c");

        test_create_destroy();
        test_insert_lookup();
        test_resize();
        test_remove();
        test_iterator();
}