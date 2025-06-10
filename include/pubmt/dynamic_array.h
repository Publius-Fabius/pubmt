#ifndef PUBMT_DYNAMIC_ARRAY_H
#define PUBMT_DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdbool.h>

/** Allocate nbytes of data. */
typedef void *(*pmt_da_alloc_t)(
        const size_t nbytes, 
        void *alloc_state);

/** Reallocate pointer to hold nbytes of data. */
typedef void *(*pmt_da_realloc_t)(
        void *pointer, 
        const size_t nbytes, 
        void *alloc_state);

/** Free the data previously allocated. */
typedef void (*pmt_da_free_t)(
        void *pointer, 
        void *alloc_state);

/** Dynamic Array Interface */
typedef struct pmt_da_iface {

        /* allocation */
        pmt_da_alloc_t (*get_alloc)(void *array);
        pmt_da_realloc_t (*get_realloc)(void *array);
        pmt_da_free_t (*get_free)(void *array);
        void *(*get_alloc_state)(void *array);

        /* metrics */
        size_t (*get_element_size)(void *array);

        /* maximum capacity */
        size_t (*get_capacity)(void *array);
        void (*set_capacity)(void *array, const size_t capacity);

        /* current size */
        size_t (*get_size)(void *array);
        void (*set_size)(void *array, const size_t size);

        /** memory region */
        void *(*get_buffer)(void *array);
        void (*set_buffer)(void *array, void *buffer_ptr);

} pmt_da_iface_t;

/**
 * Validate the dynamic array interface. 
 * @returns Will return 'false' if any callbacks are NULL.
 */
bool pmt_da_iface_validate(pmt_da_iface_t *iface);

/** 
 * Initialize the dynamic array with the given buffer and initial capacity. 
 * 
 * @returns A pointer to 'array'.
 */
void *pmt_da_init(
        pmt_da_iface_t *iface, 
        void *array, 
        void *buffer,
        const size_t size,
        const size_t initial_capacity);

/** 
 * Create a new dynamic array with the given initial capacity.
 * 
 * @returns A pointer to 'array' or NULL if memory allocation failed.
 */
void *pmt_da_create(
        pmt_da_iface_t *iface, 
        void *array, 
        const size_t initial_capacity);

/** 
 * Destroy the dynamic array, freeing its internal buffer.
 */    
void pmt_da_destroy(pmt_da_iface_t *iface, void *array);

/** 
 * Clear the dynamic array, removing all its elements.
 */
void pmt_da_clear(pmt_da_iface_t *iface, void *array);

/** 
 * Zero internal buffer memory length elems from index.  This operation 
 * ignores the array's current size.
 * 
 * @returns If 'false' is returned the operation would have gone out of
 * bounds, no memory would be zeroed in this case.
 */
bool pmt_da_zero_buffer(
        pmt_da_iface_t *iface, 
        void *array, 
        const size_t index, 
        const size_t length);

/** 
 * Is the dynamic array empty O(1)? 
 * 
 * @returns A true value indicates the array was empty, otherwise false.
 */
bool pmt_da_is_empty(pmt_da_iface_t *iface, void *array);

/**
 * Get the element at the given index O(1). 
 * 
 * @returns A pointer to the element is returned.  If the 'index' is out of 
 * bounds, then NULL is returned instead.
 */
void *pmt_da_at(pmt_da_iface_t *iface, void *array, const size_t index);

/** 
 * Resize the array, ensuring it can hold new_capacity elements.
 * 
 * @returns A value of 'false' is returned if there was a memory allocation 
 * error.
 */
bool pmt_da_resize(
        pmt_da_iface_t *iface, 
        void *array, 
        const size_t new_capacity);

/**
 * Shrink the array's memory buffer to fit its current size. 
 * 
 * @returns A value of 'false' is returned if there was a memory allocation 
 * error.
 */
bool pmt_da_shrink_to_fit(pmt_da_iface_t *iface, void *array);

/**
 * Reserve nelems of internal buffer space.
 * 
 * @returns A value of 'false' is returned if there was a memory allocation 
 * error.
 */
bool pmt_da_reserve( pmt_da_iface_t *iface, void *array, const size_t nelems);

/**
 * Increase the capacity by a natural number power of the array's scaling 
 * factor to ensure the given capacity.
 * 
 * @returns A value of 'false' indicates a memory allocation failure.  If the 
 * array's scaling factor is less than or equal to 1, then false is returned.
 */
bool pmt_da_scale_capacity(
        pmt_da_iface_t *iface, 
        void *array,
        const size_t ensured_capacity);

/**
 * Push an element onto the end of the array.  This function may resize the 
 * internal buffer.  If 'element' is non-NULL, its contents are copied into 
 * the pushed region.
 * 
 * @returns A pointer to the pushed element is returned.  A value of 'NULL' 
 * indicates a memory allocation failure.
 */
void *pmt_da_push_back(pmt_da_iface_t *iface, void *array, void *element);

/**
 * Remove the last element from the array.  If element is not NULL, then it 
 * will receive a copy of the popped elements contents.
 * 
 * @returns If false is returned, the array was empty.
 */
bool pmt_da_pop_back(pmt_da_iface_t *iface, void *array, void *element);

/**
 * Get a pointer to the first element. 
 * 
 * @returns A pointer to the first element is returned, other NULL if the array
 * is empty.
 */
void *pmt_da_first(pmt_da_iface_t *iface, void *array);

/**
 * Get a pointer to the last element. 
 * 
 * @returns A pointer to the last element is returned, other NULL if the array
 * is empty.
 */
void *pmt_da_last(pmt_da_iface_t *iface, void *array);

/**
 * Insert nelems elements at the given index. 
 * 
 * @returns A value of false is returned if the allocator ran out of memory.
 */
bool pmt_da_insert_range(
        pmt_da_iface_t *iface, 
        void *array, 
        const size_t index,
        void *elements, 
        const size_t nelems);

/** 
 * Remove nelems elements starting at the given index.  
 * 
 * @returns A value of false is returned if the operation would have otherwise 
 * gone out of bounds, indicating no modifications were made to the array.
 */
bool pmt_da_remove_range(
        pmt_da_iface_t *iface, 
        void *array,
        const size_t index, 
        const size_t nelems);

#endif