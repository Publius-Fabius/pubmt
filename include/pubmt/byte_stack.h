#ifndef PUBMT_BYTE_STACK_H
#define PUBMT_BYTE_STACK_H

#include <stddef.h>
#include <stdbool.h>

/** Downward Growing Byte Stack */
typedef struct pmt_byte_stack {

        size_t length, offset;

        void *buffer;

} pmt_byte_stack_t;

/**
 * Initialize a new byte stack with the given buffer, length, and offset.  
 * This stack grows downward, therefore, an empty stack has an offset equal
 * to its length.
 * 
 * @returns A pointer to the stack is returned.
 */
pmt_byte_stack_t *pmt_bs_init(
        pmt_byte_stack_t *stack, 
        void *buffer,
        const size_t offset,
        const size_t length);

/** 
 * Reset the stack and zero its buffer.
 * 
 * @returns A pointer to the stack is returned.
 */
pmt_byte_stack_t *pmt_bs_zero(pmt_byte_stack_t *stack);

/** 
 * Reset the stack, removing all its contents. 
 * 
 * @returns A pointer to the stack is returned.
 */
pmt_byte_stack_t *pmt_bs_reset(pmt_byte_stack_t *stack);

/** 
 * Push nbytes onto the stack.  If the argument 'data' is not NULL, then the
 * pushed region will receive its contents.
 * 
 * @returns A pointer to the pushed region.
 */
void *pmt_bs_push(pmt_byte_stack_t *stack, const size_t nbytes, void *data);

/** 
 * Pop nbytes from the stack.  If the argument 'data' is not NULL, then 
 * it will receive the popped region's contents.
 * 
 * @returns If 'false' is returned, the operation would have gone out of 
 * bounds, in which case the stack is left unmodified.
 */
bool pmt_bs_pop(pmt_byte_stack_t *stack, const size_t nbytes, void *data);

/** 
 * Get a pointer to the top of the stack.
 * 
 * @returns A value of 'false' is returned when the stack is empty.
 */
void *pmt_bs_peek(pmt_byte_stack_t *stack);

/**
 * Is the stack empty?
 * 
 * @returns A value of 'true' is returned when the stack is empty.
 */
bool pmt_bs_is_empty(pmt_byte_stack_t *stack);

#endif