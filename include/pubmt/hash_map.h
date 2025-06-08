#ifndef PUBMT_HASH_MAP_H
#define PUBMT_HASH_MAP_H

#include "pubmt/dynamic_array.h"
#include "pubmt/linked_list.h"

/** Are the nodes equal? */
typedef bool (*pmt_hm_equals_t)(void *key_a, void *key_b);

/** Hash the node. */
typedef size_t (*pmt_hm_hash_t)(void *node);

/** Hash Map Callback Interface */
typedef struct pmt_hm_iface {
        
        pmt_ll_iface list_iface;

        pmt_da_iface array_iface;

        void *get_key(void *node);

        pmt_hm_equals_t get_equals(void *map);

        pmt_hm_hash_t get_hash(void *map);

        size_t get_load_factor_numerator(void *map);
        
        size_t get_load_factor_denominator(void *map);

} pmt_hm_iface;

/** Hash Map Iterator */
typedef struct pmt_hm_iter {

        size_t bucket;
        
        void *node;

        void *map;

} pmt_hm_iter_t;

void *pmt_hm_create(pmt_hm_iface *iface, const size_t initial_capacity);

void *pmt_hm_destroy(pmt_hm_iface *iface, void *map);

void *pmt_hm_insert(pmt_hm_iface *iface, void *map, void *node);

void *pmt_hm_lookup(pmt_hm_iface *iface, void *map, void *key);

void *pmt_hm_remove(pmt_hm_iface *iface, void *map, void *key);

void *pmt_hm_update(
        pmt_hm_iface *iface, 
        void *map, 
        void *key, 
        void *(callback)(void *node, void *state),
        void *state);

void *pmt_hm_entries(pmt_hm_iface *iface, void *map, pmt_hm_iter_t *iter);

void *pmt_hm_from(
        pmt_hm_iface *iface, 
        void *map, 
        pmt_hm_iter_t *iter, 
        void *key);

bool pmt_hm_next(pmt_hm_iface *iface, pmt_hm_iter_t *iter, void **node);

size_t law_hm_fnv(void *src, const size_t nb);

#endif 