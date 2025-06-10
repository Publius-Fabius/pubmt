
#include "pubmt/byte_stack.h"
#include <string.h>
#include <stdint.h>
#include <assert.h>

pmt_byte_stack_t *pmt_bs_init(
        pmt_byte_stack_t *stack, 
        void *buffer,
        const size_t offset,
        const size_t length)
{
        assert(stack && offset <= length);

        stack->buffer = buffer;
        stack->length = length;
        stack->offset = offset;

        return stack;
}

pmt_byte_stack_t *pmt_bs_zero(pmt_byte_stack_t *stack)
{
        assert(stack);

        (void)memset(stack->buffer, 0, stack->length);

        stack->offset = stack->length;
        
        return stack;
}

pmt_byte_stack_t *pmt_bs_reset(pmt_byte_stack_t *stack)
{
        assert(stack);

        stack->offset = stack->length;

        return stack;
}

bool pmt_bs_is_empty(pmt_byte_stack_t *stack)
{
        assert(stack);

        return stack->offset == stack->length;
}

void *pmt_bs_push(pmt_byte_stack_t *stack, const size_t nbytes, void *data)
{
        assert(stack && stack->offset <= stack->length);

        if(stack->offset < nbytes) {
                return NULL;
        }

        stack->offset -= nbytes;

        void *pointer = ((uint8_t*)stack->buffer) + stack->offset;

        if(data) {
                (void)memcpy(pointer, data, nbytes);
        }

        return pointer;
}

void *pmt_bs_peek(pmt_byte_stack_t *stack)
{
        assert(stack && stack->offset <= stack->length);

        return ((uint8_t*)stack->buffer) + stack->offset;
}

bool pmt_bs_pop(pmt_byte_stack_t *stack, const size_t nbytes, void *data)
{
        assert(stack && stack->offset <= stack->length);

        const size_t new_offset = stack->offset + nbytes; 

        if(new_offset > stack->length) {
                return false;
        }

        if(data) {
                (void)memcpy(
                        data, 
                        ((uint8_t*)stack->buffer) + stack->offset, 
                        nbytes);
        }

        stack->offset = new_offset;

        return true;
}
