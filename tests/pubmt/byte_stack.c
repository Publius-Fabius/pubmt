
#include "pubmt/byte_stack.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void test_init()
{
        char buffer[8];
        pmt_byte_stack_t stack;

        assert(pmt_bs_init(&stack, buffer, 4, 8) == &stack);
        assert(stack.buffer == buffer);
        assert(stack.offset = 4);
        assert(stack.length = 8);
}

void test_zero()
{
        char *buf = malloc(8);

        for(int x = 0; x < 8; ++x) {
                buf[x] = (char)x;
        }

        pmt_byte_stack_t stack;
        assert(pmt_bs_init(&stack, buf, 8, 8) == &stack);
        assert(pmt_bs_zero(&stack) == &stack);

        for(int x = 0; x < 8; ++x) {
                buf[x] = (char)0;
        }

        free(buf);
}

void test_push()
{
        char *buf = malloc(8);

        pmt_byte_stack_t stack;
        assert(pmt_bs_init(&stack, buf, 8, 8) == &stack);

        for(char x = 0; x < 8; ++x) {
                assert(pmt_bs_push(&stack, 1, (void*)&x));
        }

        assert(!pmt_bs_push(&stack, 1, NULL));

        for(int x = 0; x < 8; ++x) {
                assert(buf[x] == (char)(7 - x));
        }

        free(buf);
}

void test_is_empty()
{
        char buffer[8];
        pmt_byte_stack_t stack;

        assert(pmt_bs_init(&stack, buffer, 8, 8) == &stack);
        assert(pmt_bs_is_empty(&stack));
        assert(pmt_bs_push(&stack, 1, NULL));
        assert(!pmt_bs_is_empty(&stack));
}

void test_reset()
{
        char buffer[8];
        pmt_byte_stack_t stack;

        assert(pmt_bs_init(&stack, buffer, 8, 8) == &stack);
        assert(pmt_bs_is_empty(&stack));
        assert(pmt_bs_push(&stack, 1, NULL));
        assert(!pmt_bs_is_empty(&stack));
        assert(pmt_bs_reset(&stack) == &stack);\
        assert(pmt_bs_is_empty(&stack));
}

void test_pop()
{
        char *buf = malloc(8);

        pmt_byte_stack_t stack;
        assert(pmt_bs_init(&stack, buf, 8, 8) == &stack);

        for(char x = 0; x < 8; ++x) {
                assert(pmt_bs_push(&stack, 1, (void*)&x));
        }

        assert(!pmt_bs_push(&stack, 1, NULL));

        for(int x = 0; x < 8; ++x) {
                char value = -1;
                assert(pmt_bs_pop(&stack, 1, (void*)&value));
                assert(value == (char)(7 - x));
        }

        free(buf);
}

void test_peek()
{
        char *buf = malloc(8);

        pmt_byte_stack_t stack;
        assert(pmt_bs_init(&stack, buf, 8, 8) == &stack);

        for(char x = 0; x < 8; ++x) {
                assert(pmt_bs_push(&stack, 1, (void*)&x));
        }

        assert(!pmt_bs_push(&stack, 1, NULL));

        for(int x = 0; x < 8; ++x) {
                char value = -1;
                char *pointer = pmt_bs_peek(&stack);
                assert(*pointer == (char)(7 - x));
                assert(pmt_bs_pop(&stack, 1, (void*)&value));
                assert(value == (char)(7 - x));
        }

        free(buf);
}

int main(int argc, char **args)
{
        puts("testing -- byte_stack.c ");

        test_init();
        test_zero();
        test_push();
        test_is_empty();
        test_reset();
        test_pop();
        test_peek();
}