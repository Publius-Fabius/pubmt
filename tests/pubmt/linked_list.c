
#include "pubmt/linked_list.h"
#include <stdio.h>
#include <assert.h>

typedef struct my_node {
        int value;
        struct my_node *next;
} my_node;

void *get_next(void *node)
{
        return ((my_node*)(node))->next;
}

void set_next(void *node, void *next)
{
        ((my_node*)(node))->next = next;
}

pmt_ll_node_iface my_node_iface = {
        .get_next = get_next,
        .set_next = set_next
};

bool match_list(my_node *list, int *nums, const size_t nnums)
{
        for(int x = 0; x < nnums; ++x) {
                if(!list) {
                        return false;
                } else if(list->value != nums[x]) {
                        return false;
                }
                list = list->next;
        }
        return true;
}

void test_push_front()
{
        my_node n1 = { .value = 1, .next = NULL };
        my_node n2 = { .value = 2, .next = NULL };
        my_node n3 = { .value = 3, .next = NULL };
        assert(pmt_ll_node_push_front(&my_node_iface, NULL, &n3) == &n3);
        assert(pmt_ll_node_push_front(&my_node_iface, &n3, &n2) == &n2);
        assert(pmt_ll_node_push_front(&my_node_iface, &n2, &n1) == &n1);
        assert(match_list(&n1, (int[]){ 1, 2, 3 }, 3));
}

void test_last()
{
        my_node n1 = { .value = 1, .next = NULL };
        my_node n2 = { .value = 2, .next = NULL };
        my_node n3 = { .value = 3, .next = NULL };
        assert(pmt_ll_node_last(&my_node_iface, NULL) == NULL);
        assert(pmt_ll_node_push_front(&my_node_iface, NULL, &n3) == &n3);
        assert(pmt_ll_node_last(&my_node_iface, &n3) == &n3);
        assert(pmt_ll_node_push_front(&my_node_iface, &n3, &n2) == &n2);
        assert(pmt_ll_node_last(&my_node_iface, &n3) == &n3);
        assert(pmt_ll_node_push_front(&my_node_iface, &n2, &n1) == &n1);
        assert(pmt_ll_node_last(&my_node_iface, &n3) == &n3);
        assert(match_list(&n1, (int[]){ 1, 2, 3 }, 3));
}

void test_count()
{
        my_node n1 = { .value = 1, .next = NULL };
        my_node n2 = { .value = 2, .next = NULL };
        my_node n3 = { .value = 3, .next = NULL };
        assert(pmt_ll_node_count(&my_node_iface, NULL) == 0);
        assert(pmt_ll_node_push_front(&my_node_iface, NULL, &n3) == &n3);
        assert(pmt_ll_node_count(&my_node_iface, &n3) == 1);
        assert(pmt_ll_node_push_front(&my_node_iface, &n3, &n2) == &n2);
        assert(pmt_ll_node_count(&my_node_iface, &n2) == 2);
        assert(pmt_ll_node_push_front(&my_node_iface, &n2, &n1) == &n1);
        assert(pmt_ll_node_count(&my_node_iface, &n1) == 3);
        assert(match_list(&n1, (int[]){ 1, 2, 3 }, 3));
}

void test_at()
{
        my_node n1 = { .value = 1, .next = NULL };
        my_node n2 = { .value = 2, .next = NULL };
        my_node n3 = { .value = 3, .next = NULL };

        assert(pmt_ll_node_at(&my_node_iface, NULL, 0) == 0);
        
        assert(pmt_ll_node_push_front(&my_node_iface, NULL, &n3) == &n3);
        assert(pmt_ll_node_at(&my_node_iface, &n3, 0) == &n3);
        assert(pmt_ll_node_at(&my_node_iface, &n3, 1) == NULL);
        
        assert(pmt_ll_node_push_front(&my_node_iface, &n3, &n2) == &n2);
        assert(pmt_ll_node_at(&my_node_iface, &n2, 0) == &n2);
        assert(pmt_ll_node_at(&my_node_iface, &n2, 1) == &n3);
        assert(pmt_ll_node_at(&my_node_iface, &n2, 2) == NULL);

        assert(pmt_ll_node_push_front(&my_node_iface, &n2, &n1) == &n1);
        assert(pmt_ll_node_at(&my_node_iface, &n1, 0) == &n1);
        assert(pmt_ll_node_at(&my_node_iface, &n1, 1) == &n2);
        assert(pmt_ll_node_at(&my_node_iface, &n1, 2) == &n3);
        assert(pmt_ll_node_at(&my_node_iface, &n1, 3) == NULL);
}

void test_push_back()
{
        my_node n1 = { .value = 1, .next = NULL };
        my_node n2 = { .value = 2, .next = NULL };
        my_node n3 = { .value = 3, .next = NULL };
        assert(pmt_ll_node_push_back(&my_node_iface, NULL, &n1) == &n1);
        assert(match_list(&n1, (int[]){ 1 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, &n1) == 1);
        assert(pmt_ll_node_push_back(&my_node_iface, &n1, &n2) == &n1);
        assert(match_list(&n1, (int[]){ 1, 2 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, &n1) == 2);
        assert(pmt_ll_node_push_back(&my_node_iface, &n1, &n3) == &n1);
        assert(match_list(&n1, (int[]){ 1, 2, 3 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, &n1) == 3);
}

void test_insert_after()
{
        my_node n1 = { .value = 1, .next = NULL };
        my_node n2 = { .value = 2, .next = NULL };
        my_node n3 = { .value = 3, .next = NULL };

        assert(pmt_ll_node_insert_after(&my_node_iface, &n1, &n3) == &n1);
        assert(match_list(&n1, (int[]){ 1, 3 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, &n1) == 2);

        assert(pmt_ll_node_insert_after(&my_node_iface, &n1, &n2) == &n1);
        assert(match_list(&n1, (int[]){ 1, 2, 3 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, &n1) == 3);
}

void test_remove_after()
{
        my_node n1 = { .value = 1, .next = NULL };
        my_node n2 = { .value = 2, .next = NULL };
        my_node n3 = { .value = 3, .next = NULL };
        my_node n4 = { .value = 4, .next = NULL };
        assert(pmt_ll_node_push_front(&my_node_iface, &n4, &n3) == &n3);
        assert(pmt_ll_node_push_front(&my_node_iface, &n3, &n2) == &n2);
        assert(pmt_ll_node_push_front(&my_node_iface, &n2, &n1) == &n1);
        assert(match_list(&n1, (int[]){ 1, 2, 3, 4 }, 4));

        assert(pmt_ll_node_remove_after(&my_node_iface, &n4) == NULL);

        assert(pmt_ll_node_remove_after(&my_node_iface, &n2) == &n3);
        assert(match_list(&n1, (int[]){ 1, 2, 4 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, &n1) == 3);

        assert(pmt_ll_node_remove_after(&my_node_iface, &n1) == &n2);
        assert(match_list(&n1, (int[]){ 1, 4 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, &n1) == 2);

        assert(pmt_ll_node_remove_after(&my_node_iface, &n1) == &n4);
        assert(match_list(&n1, (int[]){ 1 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, &n1) == 1);

        assert(pmt_ll_node_remove_after(&my_node_iface, &n1) == NULL);
}

int main(int argc, char **args) 
{
        puts("Testing linked_list.c");
        test_push_front();
        test_last();
        test_count();
        test_at();
        test_push_back();
        test_insert_after();
        test_remove_after();
        test_remove_first();
        return 0;
}