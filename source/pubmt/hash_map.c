
#include "pubmt/hash_map.h"
#include <assert.h>
#include <string.h>
#include <stdint.h>

bool pmt_hm_iface_validate(pmt_hm_iface_t *iface)
{
        return 
                iface &&
                iface->get_key &&
                iface->get_equals && 
                iface->get_hash &&
                pmt_da_iface_validate(&iface->array_iface) &&
                pmt_ll_node_iface_validate(&iface->node_iface);
}

void *pmt_hm_create(
        pmt_hm_iface_t *iface, 
        void *map, 
        const size_t init_cap)
{
        assert(iface);

        if(!pmt_da_create(&iface->array_iface, map, init_cap)) {
                return NULL;
        }

        (void)pmt_da_zero_buffer(&iface->array_iface, map, 0, init_cap);

        return map;
}

void pmt_hm_destroy(pmt_hm_iface_t *iface, void *map)
{
        assert(iface);

        pmt_da_destroy(&iface->array_iface, map);
}

bool pmt_hm_resize(pmt_hm_iface_t *iface, void *map, const size_t new_cap)
{
        assert(map && pmt_hm_iface_validate(iface));

        pmt_da_iface_t *array_iface = &iface->array_iface;
        pmt_ll_node_iface_t *node_iface = &iface->node_iface;

        assert(array_iface->get_capacity(map) <= new_cap);

        pmt_da_alloc_t alloc = array_iface->get_alloc(map);
        pmt_da_free_t free = array_iface->get_free(map);
        void *alloc_state = array_iface->get_alloc_state(map);
        const size_t new_len = new_cap * sizeof(void*);

        void **new_buf = alloc(new_len, alloc_state);
        if(!new_buf) {
                return false;
        }

        (void)memset(new_buf, 0, new_len);

        pmt_hm_hash_t hash = iface->get_hash(map);

        pmt_hm_iter_t iter;
        pmt_hm_entries(iface, map, &iter);

        void *node = NULL;
        while(pmt_hm_next(iface, &iter, &node)) {
                assert(node);
                node_iface->set_next(node, NULL);
                const size_t hash_value = hash(iface->get_key(node));
                void **bucket = new_buf + (hash_value % new_cap);
                *bucket = pmt_ll_node_push_front(node_iface, *bucket, node);
        }

        free(array_iface->get_buffer(map), alloc_state);

        array_iface->set_capacity(map, new_cap);
        array_iface->set_buffer(map, new_buf);

        return true;
}

typedef struct pmt_hm_predicate_args {

        void *key;

        pmt_hm_equals_t equals;

        void *(*get_key)(void *node);

} pmt_hm_predicate_args_t;

static bool pmt_hm_predicate(void *node, void *state)
{
        pmt_hm_predicate_args_t *args = state;
        return args->equals(args->get_key(node), args->key);
}

int pmt_hm_insert(pmt_hm_iface_t *iface, void *map, void *node)
{
        assert(map && node && pmt_hm_iface_validate(iface));

        pmt_da_iface_t *array_iface = &iface->array_iface;
        pmt_ll_node_iface_t *node_iface = &iface->node_iface;

        const size_t new_size = array_iface->get_size(map) + 1;
        
        size_t  
                capacity = array_iface->get_capacity(map),
                load = capacity * 3;

        if(load < capacity) {
                return PMT_HM_RESIZE;
        }

        load /= 4;

        if(new_size >= load) {
                capacity *= 2;
                if(!pmt_hm_resize(iface, map, capacity)) {
                        return PMT_HM_RESIZE;
                }
        }

        void    **buffer = array_iface->get_buffer(map),
                *key = iface->get_key(node),
                **bucket = buffer + iface->get_hash(map)(key) % capacity;
    
        pmt_hm_predicate_args_t args = { 
                .equals = iface->get_equals(map), 
                .get_key = iface->get_key,
                .key = key };
    
        if(pmt_ll_node_find(node_iface, *bucket, pmt_hm_predicate, &args)) {
                return PMT_HM_EXISTS;
        }

        *bucket = pmt_ll_node_push_front(node_iface, *bucket, node);

        array_iface->set_size(map, new_size);
        
        return PMT_HM_SUCCESS;
}

void *pmt_hm_lookup(pmt_hm_iface_t *iface, void *map, void *key)
{
        assert(map && key && pmt_hm_iface_validate(iface));

        pmt_da_iface_t *array_iface = &iface->array_iface;
        pmt_ll_node_iface_t *node_iface = &iface->node_iface;

        const size_t capacity = array_iface->get_capacity(map);

        void    
                **buffer = array_iface->get_buffer(map),
                *bucket = buffer[iface->get_hash(map)(key) % capacity];
    
        pmt_hm_predicate_args_t args = { 
                .equals = iface->get_equals(map), 
                .get_key = iface->get_key,
                .key = key };

        return pmt_ll_node_find(node_iface, bucket, pmt_hm_predicate, &args);
}

void *pmt_hm_remove(pmt_hm_iface_t *iface, void *map, void *key)
{
        assert(map && key && pmt_hm_iface_validate(iface));

        pmt_da_iface_t *array_iface = &iface->array_iface;
        pmt_ll_node_iface_t *node_iface = &iface->node_iface;

        const size_t capacity = array_iface->get_capacity(map);

        pmt_hm_predicate_args_t args = { 
                .equals = iface->get_equals(map), 
                .get_key = iface->get_key,
                .key = key };

        void    **buffer = array_iface->get_buffer(map),
                **bucket = buffer + (iface->get_hash(map)(key) % capacity);
        
        void *removed_node = pmt_ll_node_remove_when(
                node_iface, 
                bucket, 
                pmt_hm_predicate, 
                &args);
        
        if(!removed_node) {
                return NULL;
        }

        array_iface->set_size(map, array_iface->get_size(map) - 1);

        return removed_node;
}

void pmt_hm_entries(pmt_hm_iface_t *iface, void *map, pmt_hm_iter_t *iter)
{
        assert(iface);
        assert(map);
        assert(iter);

        pmt_da_iface_t *array_iface = &iface->array_iface;
        assert(array_iface->get_buffer);

        iter->bucket = 0;
        iter->map = map,
        iter->node = *((void**)array_iface->get_buffer(map));
}

bool pmt_hm_next(
        pmt_hm_iface_t *iface,
        pmt_hm_iter_t *iter,
        void **node)
{
        assert(iface);
        assert(iter);

        pmt_da_iface_t *array_iface = &iface->array_iface;
        assert(array_iface->get_capacity);
        assert(array_iface->get_buffer);

        pmt_ll_node_iface_t *node_iface = &iface->node_iface;

        const size_t capacity = array_iface->get_capacity(iter->map);

        if(iter->bucket >= capacity) {
                return false;
        }

        void **buffer = array_iface->get_buffer(iter->map);

        while(!iter->node) {

                if(++iter->bucket >= capacity) {
                        return false;
                }

                iter->node = buffer[iter->bucket];
        }
     
        if(node) {
                *node = iter->node;
        }

        iter->node = node_iface->get_next(iter->node);

        return true;
}

bool pmt_hm_is_next(pmt_hm_iface_t *iface, pmt_hm_iter_t *iter)
{
        const size_t capacity = iface->array_iface.get_capacity(iter->map);
        void **buffer = iface->array_iface.get_buffer(iter->map);

        while(!iter->node) {
                if(++iter->bucket >= capacity) {
                        return false;
                }
                iter->node = buffer[iter->bucket];
        }

        return iter->bucket < capacity;
}

size_t pmt_hm_fnv(void *src, const size_t nbytes)
{
        #if UINTPTR_MAX > 14695981039346656037ULL

                const uint64_t FNV_offset_basis = 14695981039346656037ULL;
                const uint64_t FNV_prime = 1099511628211ULL;

                uint64_t hash = FNV_offset_basis;

        #elif 

                const uint32_t FNV_offset_basis = 2166136261UL;
                const uint32_t FNV_prime = 16777619UL;

                uint32_t hash = FNV_offset_basis;

        #endif

        for(size_t i = 0; i < nbytes; ++i) {
                hash ^= ((uint8_t*)src)[i];
                hash *= FNV_prime;
        }

        return (size_t)hash;
}
