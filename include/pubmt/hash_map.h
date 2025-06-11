#ifndef PUBMT_HASH_MAP_H
#define PUBMT_HASH_MAP_H

#include "pubmt/dynamic_array.h"
#include "pubmt/linked_list.h"

/** Are the nodes equal? */
typedef bool (*pmt_hm_equals_t)(void *key_a, void *key_b);

/** Hash the key. */
typedef size_t (*pmt_hm_hash_t)(void *key);

/** Hash Map Callback Interface */
typedef struct pmt_hm_iface {
        
        pmt_ll_node_iface_t node_iface;

        pmt_da_iface_t array_iface;

        void *(*get_key)(void *node);

        pmt_hm_equals_t (*get_equals)(void *map);

        pmt_hm_hash_t (*get_hash)(void *map);

} pmt_hm_iface_t;

/** Hash Map Iterator */
typedef struct pmt_hm_iter {

        size_t bucket;
        
        void *node;

        void *map;

} pmt_hm_iter_t;

/** Error Codes */
enum pmt_hm_error {
        PMT_HM_SUCCESS                  = 0,
        PMT_HM_EXISTS                   = -1,
        PMT_HM_RESIZE                   = -2
};

/**
 * Validate the hash map interface. 
 * 
 * @returns Will return 'false' if any callbacks are NULL.
 */
bool pmt_hm_iface_validate(pmt_hm_iface_t *iface);

/**
 * Create a new hash map with the given initial capacity.
 * 
 * @return map
 */
void *pmt_hm_create(
        pmt_hm_iface_t *iface, 
        void *map, 
        const size_t initial_capacity);

/**
 * Destroy the hash map.
 */
void pmt_hm_destroy(pmt_hm_iface_t *iface, void *map);

/**
 * Resize the hash map's internal buffer.
 * 
 * @returns A value of 'false' is returned when memory allocation fails.
 */
bool pmt_hm_resize(
        pmt_hm_iface_t *iface, 
        void *map, 
        const size_t new_capacity);

/**
 * Insert a node into the hash map unless a node with the same key already 
 * exists.
 * 
 * @returns 
 *      PMT_HM_SUCCESS - The node was inserted. 
 *      PMT_HM_EXISTS - Operation failed because the key already exists.
 *      PMT_HM_RESIZE - Operation failed because it couldn't resize the map.
 */
int pmt_hm_insert(pmt_hm_iface_t *iface, void *map, void *node);

/**
 * Lookup the node with the given key. 
 * 
 * @returns The node with the given key, otherwise NULL.
 */
void *pmt_hm_lookup(pmt_hm_iface_t *iface, void *map, void *key);

/**
 * Remove the node from the hash map.
 * 
 * @returns The removed node if it exists, otherwise NULL.
 */
void *pmt_hm_remove(pmt_hm_iface_t *iface, void *map, void *key);

/**
 * Get an iterator to the beginning of the hash map.
 */
void pmt_hm_entries(pmt_hm_iface_t *iface, void *map, pmt_hm_iter_t *iter);

/**
 * Get the next node in the iteration.
 * 
 * @returns A value of 'false' indicates the iteration has ended.
 */
bool pmt_hm_next(pmt_hm_iface_t *iface, pmt_hm_iter_t *iter, void **node);

/** 
 * Does the iterator have a next node?
 */
bool pmt_hm_is_next(pmt_hm_iface_t *iface, pmt_hm_iter_t *iter);

/**
 * FNV hash nbytes of src.
 * 
 * @returns The hashed value returns.
 */
size_t pmt_hm_fnv(void *src, const size_t nbytes);

#endif 