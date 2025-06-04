
#include "pubmt/linked_list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

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

void *init_list(my_node *list, const size_t nnodes) 
{
        memset(list, 0, sizeof(my_node) * nnodes);
        my_node *prev = NULL;
        for(size_t x = 0; x < nnodes; ++x) {
                my_node *node = list + x;
                node->next = prev;
                node->value = (int)(nnodes - x);
                prev = node;
        }
        return prev;
}

bool equals(void *node, void *state)
{
        my_node *n = node;
        return *((int*)state) == n->value;
}

bool evens(void *node, void *state)
{
        return ((my_node*)node)->value % 2 == 0;
}

bool odds(void *node, void *state)
{
        return ((my_node*)node)->value % 2 == 1;
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
void test_remove_first()
{
        my_node nodes[5];
        my_node *list = init_list(nodes, 2);
        assert(match_list(list, (int[]){ 1, 2 }, 2));

        my_node *ref = list;
        my_node *next = list->next;

        assert(pmt_ll_node_remove_first(&my_node_iface, (void**)&ref) == list);
        assert(list->next == NULL);
        assert(match_list(ref, (int[]){ 2 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 1);

        assert(pmt_ll_node_remove_first(&my_node_iface, (void**)&ref) == next);
        assert(next->next == NULL);
        assert(ref == NULL);

        assert(pmt_ll_node_remove_first(&my_node_iface, (void**)&ref) == NULL);
}

void test_remove_last()
{
        my_node nodes[5];
        my_node *list = init_list(nodes, 2);
        assert(match_list(list, (int[]){ 1, 2 }, 2));

        my_node *ref = list;
        my_node *last;

        assert((last = pmt_ll_node_remove_last(&my_node_iface, (void**)&ref)));
        assert(last->next == NULL);
        assert(last->value == 2);
        assert(match_list(ref, (int[]){ 1 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 1);

        assert((last = pmt_ll_node_remove_last(&my_node_iface, (void**)&ref)));
        assert(last->next == NULL);
        assert(last->value == 1);
        assert(ref == NULL);

        assert(pmt_ll_node_remove_last(&my_node_iface, (void**)&ref) == NULL);
}

void test_remove()
{
        my_node nodes[5];
        my_node *list = init_list(nodes, 5);
        assert(match_list(list, (int[]){ 1, 2, 3, 4, 5 }, 5));

        my_node *ref = list;

        my_node *rem = pmt_ll_node_at(&my_node_iface, ref, 2);

        assert(pmt_ll_node_remove(&my_node_iface, (void**)&ref, rem) == rem);
        assert(match_list(ref, (int[]){ 1, 2, 4, 5 }, 4));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 4);
        
        rem = pmt_ll_node_at(&my_node_iface, ref, 3);
        assert(pmt_ll_node_remove(&my_node_iface, (void**)&ref, rem) == rem);
        assert(match_list(ref, (int[]){ 1, 2, 4 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 3);
        
        rem = pmt_ll_node_at(&my_node_iface, ref, 0);
        assert(pmt_ll_node_remove(&my_node_iface, (void**)&ref, rem) == rem);
        assert(match_list(ref, (int[]){ 2, 4 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 2);

        rem = pmt_ll_node_at(&my_node_iface, ref, 1);
        assert(pmt_ll_node_remove(&my_node_iface, (void**)&ref, rem) == rem);
        assert(match_list(ref, (int[]){ 2 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 1);

        rem = pmt_ll_node_at(&my_node_iface, ref, 0);
        assert(pmt_ll_node_remove(&my_node_iface, (void**)&ref, rem) == rem);
        assert(ref == NULL);

        assert(pmt_ll_node_remove(&my_node_iface, (void**)&ref, rem) == NULL);
        assert(ref == NULL);
}

void test_remove_when()
{
        my_node nodes[5];
        my_node *list = init_list(nodes, 5);
        assert(match_list(list, (int[]){ 1, 2, 3, 4, 5 }, 5));

        my_node *ref = list;
        my_node *rem = NULL;

        int value = 3;
        assert((rem = pmt_ll_node_remove_when(
                &my_node_iface, (void**)&ref, equals, (void*)&value)));
        assert(rem->value == 3);
        assert(match_list(list, (int[]){ 1, 2, 4, 5 }, 4));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 4);

        value = 5;
        assert((rem = pmt_ll_node_remove_when(
                &my_node_iface, (void**)&ref, equals, (void*)&value)));
        assert(rem->value == 5);
        assert(match_list(ref, (int[]){ 1, 2, 4 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 3);

        value = 2;
        assert((rem = pmt_ll_node_remove_when(
                &my_node_iface, (void**)&ref, equals, (void*)&value)));
        assert(rem->value == 2);
        assert(match_list(ref, (int[]){ 1, 4 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 2);

        value = 10;
        assert(pmt_ll_node_remove_when(
                &my_node_iface, (void**)&ref, equals, (void*)&value) == NULL);
        assert(ref == list);

        value = 1;
        assert((rem = pmt_ll_node_remove_when(
                &my_node_iface, (void**)&ref, equals, (void*)&value)));
        assert(rem->value == 1);
        assert(match_list(ref, (int[]){ 4 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 1);

        value = 4;
        assert((rem = pmt_ll_node_remove_when(
                &my_node_iface, (void**)&ref, equals, (void*)&value)));
        assert(rem->value == 4);
        assert(pmt_ll_node_count(&my_node_iface, ref) == 0);

        assert( pmt_ll_node_remove_when(
                &my_node_iface, (void**)&ref, equals, (void*)&value) == NULL);
}

void test_filter()
{
        my_node nodes[6];
        my_node *list = init_list(nodes, 6);
        assert(match_list(list, (int[]){ 1, 2, 3, 4, 5, 6 }, 6));

        my_node *ref = list;

        assert(pmt_ll_node_filter(&my_node_iface, (void**)&ref, evens, NULL) == 3);
        assert(match_list(ref, (int[]){ 2, 4, 6 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 3);

        ref = init_list(nodes, 6);
        assert(pmt_ll_node_filter(&my_node_iface, (void**)&ref, odds, NULL) == 3);
        assert(match_list(list, (int[]){ 1, 3, 5 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 3);
}

void test_find()
{
        my_node nodes[6];
        my_node *list = init_list(nodes, 6);
        assert(match_list(list, (int[]){ 1, 2, 3, 4, 5, 6 }, 6));
        my_node *node;

        int num = 3;
        assert((node = pmt_ll_node_find(&my_node_iface, list, equals, &num)));
        assert(node->value == 3);

        num = 1;
        assert((node = pmt_ll_node_find(&my_node_iface, list, equals, &num)));
        assert(node->value == 1);

        num = 6;
        assert((node = pmt_ll_node_find(&my_node_iface, list, equals, &num)));
        assert(node->value == 6);

        num = 7;
        assert(pmt_ll_node_find(&my_node_iface, list, equals, &num) == NULL);
}

void test_foreach()
{
        my_node nodes[6];
        my_node *list = init_list(nodes, 6);
        assert(match_list(list, (int[]){ 1, 2, 3, 4, 5, 6 }, 6));

        my_node *ref = list;
        assert(pmt_ll_node_filter(&my_node_iface, (void**)&ref, evens, NULL) == 3);
        assert(pmt_ll_node_foreach(&my_node_iface, ref, evens, NULL));
        assert(pmt_ll_node_foreach(&my_node_iface, ref, odds, NULL) == false);
}

void test_reverse()
{
        my_node nodes[6];
        my_node *list = init_list(nodes, 5);
        assert(match_list(list, (int[]){ 1, 2, 3, 4, 5 }, 5));

        assert((list = pmt_ll_node_reverse(&my_node_iface, list)));
        assert(match_list(list, (int[]){ 5, 4, 3, 2, 1 }, 5));
        assert(pmt_ll_node_count(&my_node_iface, list) == 5);

        list = init_list(nodes, 2);
        assert(match_list(list, (int[]){ 1, 2 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, list) == 2);
        assert((list = pmt_ll_node_reverse(&my_node_iface, list)));
        assert(match_list(list, (int[]){ 2, 1 }, 2));

        list = init_list(nodes, 1);
        assert(match_list(list, (int[]){ 1 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, list) == 1);
        assert((list = pmt_ll_node_reverse(&my_node_iface, list)));
        assert(match_list(list, (int[]){ 1 }, 1));
}

int main(int argc, char **args) 
{
        puts("testing - linked_list.c");
        test_push_front();
        test_last();
        test_count();
        test_at();
        test_push_back();
        test_insert_after();
        test_remove_after();
        test_remove_first();
        test_remove_last();
        test_remove();
        test_remove_when();
        test_filter();
        test_find();
        test_foreach();
        test_reverse();
        return 0;
}