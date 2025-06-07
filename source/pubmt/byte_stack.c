
#include "pubmt/byte_stack.h"
#include <string.h>

pmt_byte_stack_t *pmt_bs_init(
        pmt_byte_stack_t *stack, 
        void *buffer,
        const size_t length, 
        const size_t offset)
{
        assert(stack);
        assert(offset <= length);

        stack->buffer = buffer;
        stack->length = length;
        stack->offset = offset;

        return stack;
}

pmt_byte_stack_t *pmt_da_zero(pmt_byte_stack_t *stack)
{
        assert(stack);

        stack->offset = stack->length;
        (void)memset(stack->buffer, 0, stack->length);

        return stack;
}

pmt_byte_stack_t *pmt_bs_reset(pmt_byte_stack_t *stack)
{
        assert(stack);

        stack->offset = stack->length;

        return stack;
}

void *pmt_bs_push(pmt_byte_stack_t *stack, const size_t nbytes)
{

}

bool pmt_bs_pop(pmt_byte_stack_t *stack, const size_t nbytes, void *data)
{

}

void *pmt_bs_peek(pmt_byte_stack_t *stack)
{

}

bool pmt_bs_is_empty(pmt_byte_stack_t *stack)
{
        
}
