#ifndef PUBMT_DYNAMIC_ARRAY_H
#define PUBMT_DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdbool.h>

/** Allocate nbytes of data. */
typedef void *(*pmt_da_alloc_t)(const size_t nbytes, void *alloc_state);

/** Free the data previously allocated. */
typedef void (*pmt_da_free_t)(void *alloc_state);

/** Dynamic Array Interface */
typedef struct pmt_da_iface {

        /* allocation */
        pmt_da_alloc_t get_alloc(void *array);
        pmt_da_free_t get_free(void *array);

        /* metrics */
        size_t get_scaling_factor(void *array);
        size_t get_element_size(void *array);

        /* maximum capacity */
        size_t get_capacity(void *array);
        void set_capacity(void *array, const size_t capacity);

        /* current size */
        size_t get_size(void *array);
        void set_size(void *array, const size_t size);

        /** memory region */
        void *get_buffer(void *array);
        void set_buffer(void *array, void *buffer_ptr);

} pmt_da_iface;

/** 
 * Initialize the dynamic array with the given buffer and initial capacity. 
 * 
 * @returns A pointer to 'array'.
 */
void *pmt_da_init(
        pmt_da_iface *iface, 
        void *array, 
        void *buffer,
        const size_t initial_capacity);

/** 
 * Create a new dynamic array with the given initial capacity.
 * 
 * @returns A pointer to 'array' or NULL if memory allocation failed.
 */
void *pmt_da_create(
        pmt_da_iface *iface, 
        void *array, 
        const size_t initial_capacity);

/** 
 * Destroy the dynamic array.
 */    
void pmt_da_destroy(pmt_da_iface *iface, void *array);

/** 
 * Clear the dynamic array. 
 */
void pmt_da_clear(pmt_da_iface *iface, void *array);

/** 
 * Is the dynamic array empty O(1)? 
 * 
 * @returns A true value indicates the array was empty, otherwise false.
 */
bool pmt_da_is_empty(pmt_da_iface *iface, void *array);

/**
 * Get the element at the given index O(1). 
 * 
 * @returns A pointer to the element is returned.  If the 'index' is out of 
 * bounds, then NULL is returned instead.
 */
void *pmt_da_at(pmt_da_iface *iface, void *array, const size_t index);

/**
 * 
 */
bool pmt_da_push_back(pmt_da_iface *iface, void *array, const void *element);

/**
 * 
 */
bool pmt_da_pop_back(void *array);

/**
 * 
 */
void *pmt_da_first(pmt_da_iface *iface);

/**
 * 
 */
void *pmt_da_last(pmt_da_iface *iface);

/**
 * 
 */
bool pmt_da_insert(
        pmt_da_iface *iface, 
        void *array, 
        const size_t index,
        void *elements, 
        const size_t nelems);

/**
 * 
 */
void pmt_da_shift_left( 
        pmt_da_iface *iface, 
        void *array,
        const size_t nelems);

/**
 * 
 */
void pmt_da_shift_right( 
        pmt_da_iface *iface, 
        void *array,
        const size_t nelems);

/**
 * 
 */
bool pmt_da_shrink_to_fit(pmt_da_iface *iface, void *array);

/**
 * 
 */
bool pmt_da_reserve( 
        pmt_da_iface *iface, 
        void *array,
        const size_t nelems);

#endif