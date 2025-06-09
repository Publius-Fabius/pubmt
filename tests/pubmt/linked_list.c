
#include "pubmt/linked_list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct my_node {
        int value;
        struct my_node *next;
} my_node_t;

void *get_next(void *node)
{
        return ((my_node_t*)(node))->next;
}

void set_next(void *node, void *next)
{
        ((my_node_t*)(node))->next = next;
}

pmt_ll_node_iface_t my_node_iface = {
        .get_next = get_next,
        .set_next = set_next
};

bool match_list(my_node_t *list, int *nums, const size_t nnums)
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

void *init_list(my_node_t *list, const size_t nnodes) 
{
        memset(list, 0, sizeof(my_node_t) * nnodes);
        my_node_t *prev = NULL;
        for(size_t x = 0; x < nnodes; ++x) {
                my_node_t *node = list + x;
                node->next = prev;
                node->value = (int)(nnodes - x);
                prev = node;
        }
        return prev;
}

bool equals(void *node, void *state)
{
        my_node_t *n = node;
        return *((int*)state) == n->value;
}

bool evens(void *node, void *state)
{
        return ((my_node_t*)node)->value % 2 == 0;
}

bool odds(void *node, void *state)
{
        return ((my_node_t*)node)->value % 2 == 1;
}


void test_node_push_front()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        assert(pmt_ll_node_push_front(&my_node_iface, NULL, &n3) == &n3);
        assert(pmt_ll_node_push_front(&my_node_iface, &n3, &n2) == &n2);
        assert(pmt_ll_node_push_front(&my_node_iface, &n2, &n1) == &n1);
        assert(match_list(&n1, (int[]){ 1, 2, 3 }, 3));
}

void test_node_last()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        assert(pmt_ll_node_last(&my_node_iface, NULL) == NULL);
        assert(pmt_ll_node_push_front(&my_node_iface, NULL, &n3) == &n3);
        assert(pmt_ll_node_last(&my_node_iface, &n3) == &n3);
        assert(pmt_ll_node_push_front(&my_node_iface, &n3, &n2) == &n2);
        assert(pmt_ll_node_last(&my_node_iface, &n3) == &n3);
        assert(pmt_ll_node_push_front(&my_node_iface, &n2, &n1) == &n1);
        assert(pmt_ll_node_last(&my_node_iface, &n3) == &n3);
        assert(match_list(&n1, (int[]){ 1, 2, 3 }, 3));
}

void test_node_count()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        assert(pmt_ll_node_count(&my_node_iface, NULL) == 0);
        assert(pmt_ll_node_push_front(&my_node_iface, NULL, &n3) == &n3);
        assert(pmt_ll_node_count(&my_node_iface, &n3) == 1);
        assert(pmt_ll_node_push_front(&my_node_iface, &n3, &n2) == &n2);
        assert(pmt_ll_node_count(&my_node_iface, &n2) == 2);
        assert(pmt_ll_node_push_front(&my_node_iface, &n2, &n1) == &n1);
        assert(pmt_ll_node_count(&my_node_iface, &n1) == 3);
        assert(match_list(&n1, (int[]){ 1, 2, 3 }, 3));
}

void test_node_at()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };

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

void test_node_push_back()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
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

void test_node_insert_after()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };

        assert(pmt_ll_node_insert_after(&my_node_iface, &n1, &n3) == &n1);
        assert(match_list(&n1, (int[]){ 1, 3 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, &n1) == 2);

        assert(pmt_ll_node_insert_after(&my_node_iface, &n1, &n2) == &n1);
        assert(match_list(&n1, (int[]){ 1, 2, 3 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, &n1) == 3);
}

void test_node_remove_after()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        my_node_t n4 = { .value = 4, .next = NULL };
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

void test_node_remove_first()
{
        my_node_t nodes[5];
        my_node_t *list = init_list(nodes, 2);
        assert(match_list(list, (int[]){ 1, 2 }, 2));

        my_node_t *ref = list;
        my_node_t *next = list->next;

        assert(pmt_ll_node_remove_first(&my_node_iface, (void**)&ref) == list);
        assert(list->next == NULL);
        assert(match_list(ref, (int[]){ 2 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 1);

        assert(pmt_ll_node_remove_first(&my_node_iface, (void**)&ref) == next);
        assert(next->next == NULL);
        assert(ref == NULL);

        assert(pmt_ll_node_remove_first(&my_node_iface, (void**)&ref) == NULL);
}

void test_node_remove_last()
{
        my_node_t nodes[5];
        my_node_t *list = init_list(nodes, 2);
        assert(match_list(list, (int[]){ 1, 2 }, 2));

        my_node_t *ref = list;
        my_node_t *last;

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

void test_node_remove_when()
{
        my_node_t nodes[5];
        my_node_t *list = init_list(nodes, 5);
        assert(match_list(list, (int[]){ 1, 2, 3, 4, 5 }, 5));

        my_node_t *ref = list;
        my_node_t *rem = NULL;

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

void test_node_filter()
{
        my_node_t nodes[6];
        my_node_t *list = init_list(nodes, 6);
        assert(match_list(list, (int[]){ 1, 2, 3, 4, 5, 6 }, 6));

        my_node_t *ref = list;

        assert(pmt_ll_node_filter(&my_node_iface, (void**)&ref, evens, NULL) == 3);
        assert(match_list(ref, (int[]){ 2, 4, 6 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 3);

        ref = init_list(nodes, 6);
        assert(pmt_ll_node_filter(&my_node_iface, (void**)&ref, odds, NULL) == 3);
        assert(match_list(list, (int[]){ 1, 3, 5 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, ref) == 3);
}

void test_node_find()
{
        my_node_t nodes[6];
        my_node_t *list = init_list(nodes, 6);
        assert(match_list(list, (int[]){ 1, 2, 3, 4, 5, 6 }, 6));
        my_node_t *node;

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

void test_node_foreach()
{
        my_node_t nodes[6];
        my_node_t *list = init_list(nodes, 6);
        assert(match_list(list, (int[]){ 1, 2, 3, 4, 5, 6 }, 6));

        my_node_t *ref = list;
        assert(pmt_ll_node_filter(&my_node_iface, (void**)&ref, evens, NULL) == 3);
        assert(pmt_ll_node_foreach(&my_node_iface, ref, evens, NULL));
        assert(pmt_ll_node_foreach(&my_node_iface, ref, odds, NULL) == false);
}

void test_node_reverse()
{
        my_node_t nodes[6];
        my_node_t *list = init_list(nodes, 5);
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

typedef struct my_list {
        my_node_t *first;
        my_node_t *last;
} my_list;

void *get_first(void *list)
{
        return ((my_list*)list)->first;
}
void *get_last(void *list)
{
        return ((my_list*)list)->last;
}

void set_first(void *list, void *node)
{
        ((my_list*)list)->first = node;
}

void set_last(void *list, void *node)
{
        ((my_list*)list)->last = node;
}


pmt_ll_iface_t my_list_iface = {
        .get_first = get_first,
        .set_first = set_first,
        .get_last = get_last,
        .set_last = set_last,
        .node_iface = {
                .get_next = get_next,
                .set_next = set_next
        }
};

void test_push_front()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        my_list list = { .first = NULL, .last = NULL };

        assert(pmt_ll_push_front(&my_list_iface, &list, &n3) == &list);
        assert(match_list(list.first, (int[]){ 3 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 1);
        assert(list.first == list.last && list.last == &n3);

        assert(pmt_ll_push_front(&my_list_iface, &list, &n2) == &list);
        assert(match_list(list.first, (int[]){ 2, 3 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 2);
        assert(list.first == &n2 && list.last == &n3);

        assert(pmt_ll_push_front(&my_list_iface, &list, &n1) == &list);
        assert(match_list(list.first, (int[]){ 1, 2, 3 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 3);
        assert(list.first == &n1 && list.last == &n3);
}

void test_push_back()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        my_list list = { .first = NULL, .last = NULL };

        assert(pmt_ll_push_back(&my_list_iface, &list, &n1) == &list);
        assert(match_list(list.first, (int[]){ 1 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 1);
        assert(list.first == list.last && list.last == &n1);

        assert(pmt_ll_push_back(&my_list_iface, &list, &n2) == &list);
        assert(match_list(list.first, (int[]){ 1, 2 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 2);
        assert(list.first == &n1 && list.last == &n2);

        assert(pmt_ll_push_back(&my_list_iface, &list, &n3) == &list);
        assert(match_list(list.first, (int[]){ 1, 2, 3 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 3);
        assert(list.first == &n1 && list.last == &n3);
}

void test_insert_after()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        my_list list = { .first = NULL, .last = NULL };

        assert(pmt_ll_push_back(&my_list_iface, &list, &n1) == &list);
        assert(match_list(list.first, (int[]){ 1 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 1);
        assert(list.first == list.last && list.last == &n1);

        assert(pmt_ll_insert_after(&my_list_iface, &list, &n1, &n3) == &list);
        assert(match_list(list.first, (int[]){ 1, 3 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 2);
        assert(list.first == &n1 && list.last == &n3);

        assert(pmt_ll_insert_after(&my_list_iface, &list, &n1, &n2) == &list);
        assert(match_list(list.first, (int[]){ 1, 2, 3 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 3);
        assert(list.first == &n1 && list.last == &n3);
}

void test_remove_after()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        my_list list = { .first = NULL, .last = NULL };

        assert(pmt_ll_push_back(&my_list_iface, &list, &n1) == &list);
        assert(match_list(list.first, (int[]){ 1 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 1);
        assert(list.first == list.last && list.last == &n1);

        assert(pmt_ll_insert_after(&my_list_iface, &list, &n1, &n3) == &list);
        assert(match_list(list.first, (int[]){ 1, 3 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 2);
        assert(list.first == &n1 && list.last == &n3);

        assert(pmt_ll_insert_after(&my_list_iface, &list, &n1, &n2) == &list);
        assert(match_list(list.first, (int[]){ 1, 2, 3 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 3);
        assert(list.first == &n1 && list.last == &n3);

        assert(pmt_ll_remove_after(&my_list_iface, &list, &n1) == &n2);
        assert(match_list(list.first, (int[]){ 1, 3 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 2);
        assert(n2.next == NULL);
        assert(list.first == &n1 && list.last == &n3);

        assert(pmt_ll_remove_after(&my_list_iface, &list, &n1) == &n3);
        assert(match_list(list.first, (int[]){ 1 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 1);
        assert(n1.next == NULL);
        assert(list.first == &n1 && list.last == &n1);
}

void test_remove_first()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        my_list list = { .first = NULL, .last = NULL };

        assert(pmt_ll_push_back(&my_list_iface, &list, &n1) == &list);
        assert(match_list(list.first, (int[]){ 1 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 1);
        assert(list.first == list.last && list.last == &n1);

        assert(pmt_ll_insert_after(&my_list_iface, &list, &n1, &n3) == &list);
        assert(match_list(list.first, (int[]){ 1, 3 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 2);
        assert(list.first == &n1 && list.last == &n3);

        assert(pmt_ll_insert_after(&my_list_iface, &list, &n1, &n2) == &list);
        assert(match_list(list.first, (int[]){ 1, 2, 3 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 3);
        assert(list.first == &n1 && list.last == &n3);

        assert(pmt_ll_remove_first(&my_list_iface, &list) == &n1);
        assert(match_list(list.first, (int[]){ 2, 3 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 2);
        assert(n1.next == NULL);
        assert(list.first == &n2 && list.last == &n3);

        assert(pmt_ll_remove_first(&my_list_iface, &list) == &n2);
        assert(match_list(list.first, (int[]){ 3 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 1);
        assert(n2.next == NULL);
        assert(list.first == &n3 && list.last == &n3);

        assert(pmt_ll_remove_first(&my_list_iface, &list) == &n3);
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 0);
        assert(n3.next == NULL);
        assert(list.first == NULL && list.last == NULL);
}

void test_remove_last()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        my_list list = { .first = NULL, .last = NULL };

        assert(pmt_ll_push_back(&my_list_iface, &list, &n1) == &list);
        assert(match_list(list.first, (int[]){ 1 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 1);
        assert(list.first == list.last && list.last == &n1);

        assert(pmt_ll_insert_after(&my_list_iface, &list, &n1, &n3) == &list);
        assert(match_list(list.first, (int[]){ 1, 3 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 2);
        assert(list.first == &n1 && list.last == &n3);

        assert(pmt_ll_insert_after(&my_list_iface, &list, &n1, &n2) == &list);
        assert(match_list(list.first, (int[]){ 1, 2, 3 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 3);
        assert(list.first == &n1 && list.last == &n3);

        assert(pmt_ll_remove_last(&my_list_iface, &list) == &n3);
        assert(match_list(list.first, (int[]){ 1, 2 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 2);
        assert(n3.next == NULL);
        assert(list.first == &n1 && list.last == &n2);

        assert(pmt_ll_remove_last(&my_list_iface, &list) == &n2);
        assert(match_list(list.first, (int[]){ 1 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 1);
        assert(n2.next == NULL);
        assert(list.first == &n1 && list.last == &n1);

        assert(pmt_ll_remove_last(&my_list_iface, &list) == &n1);
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 0);
        assert(n1.next == NULL);
        assert(list.first == NULL && list.last == NULL);
}

void test_remove_when()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        my_node_t n4 = { .value = 4, .next = NULL };
        my_node_t n5 = { .value = 5, .next = NULL };

        my_list list = { .first = NULL, .last = NULL };

        assert(pmt_ll_push_back(&my_list_iface, &list, &n1) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n2) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n3) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n4) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n5) == &list);
        assert(match_list(list.first, (int[]){ 1, 2, 3, 4, 5 }, 5));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 5);
        
        int value = 3;
        assert(pmt_ll_remove_when(
                &my_list_iface, &list, equals, &value) == &n3);
        assert(match_list(list.first, (int[]){ 1, 2, 4, 5 }, 4));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 4);
        assert(n3.next == NULL);
        assert(list.first == &n1 && list.last == &n5);

        value = 1;
        assert(pmt_ll_remove_when(
                &my_list_iface, &list, equals, &value) == &n1);
        assert(match_list(list.first, (int[]){ 2, 4, 5 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 3);
        assert(n1.next == NULL);
        assert(list.first == &n2 && list.last == &n5);

        value = 5;
        assert(pmt_ll_remove_when(
                &my_list_iface, &list, equals, &value) == &n5);
        assert(match_list(list.first, (int[]){ 2, 4 }, 2));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 2);
        assert(n5.next == NULL);
        assert(list.first == &n2 && list.last == &n4);

        value = 2;
        assert(pmt_ll_remove_when(
                &my_list_iface, &list, equals, &value) == &n2);
        assert(match_list(list.first, (int[]){ 4 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 1);
        assert(n2.next == NULL);
        assert(list.first == &n4 && list.last == &n4);

        value = 10;
        assert(pmt_ll_remove_when(
                &my_list_iface, &list, equals, &value) == NULL);
        assert(match_list(list.first, (int[]){ 4 }, 1));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 1);
        assert(n2.next == NULL);
        assert(list.first == &n4 && list.last == &n4);

        value = 4;
        assert(pmt_ll_remove_when(
                &my_list_iface, &list, equals, &value) == &n4);
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 0);
        assert(n4.next == NULL);
        assert(list.first == NULL && list.last == NULL);
}

void test_filter()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        my_node_t n4 = { .value = 4, .next = NULL };
        my_node_t n5 = { .value = 5, .next = NULL };
        my_node_t n6 = { .value = 6, .next = NULL };
        my_list list = { .first = NULL, .last = NULL };

        assert(pmt_ll_push_back(&my_list_iface, &list, &n1) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n2) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n3) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n4) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n5) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n6) == &list);
        assert(match_list(list.first, (int[]){ 1, 2, 3, 4, 5, 6 }, 6));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 6);
        
        assert(pmt_ll_filter(
                &my_list_iface, &list, evens, NULL) == 3);
        assert(match_list(list.first, (int[]){ 2, 4, 6 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 3);
        assert(list.first == &n2 && list.last == &n6);

        assert(pmt_ll_filter(
                &my_list_iface, &list, odds, NULL) == 3);
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 0);
        assert(list.first == NULL && list.last == NULL);

        assert(pmt_ll_push_back(&my_list_iface, &list, &n1) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n2) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n3) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n4) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n5) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n6) == &list);
        assert(match_list(list.first, (int[]){ 1, 2, 3, 4, 5, 6 }, 6));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 6);

        assert(pmt_ll_filter(
                &my_list_iface, &list, odds, NULL) == 3);
        assert(match_list(list.first, (int[]){ 1, 3, 5 }, 3));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 3);
        assert(list.first == &n1 && list.last == &n5);

        assert(pmt_ll_filter(
                &my_list_iface, &list, evens, NULL) == 3);
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 0);
        assert(list.first == NULL && list.last == NULL);
}

void test_reverse()
{
        my_node_t n1 = { .value = 1, .next = NULL };
        my_node_t n2 = { .value = 2, .next = NULL };
        my_node_t n3 = { .value = 3, .next = NULL };
        my_node_t n4 = { .value = 4, .next = NULL };
        my_node_t n5 = { .value = 5, .next = NULL };
        my_node_t n6 = { .value = 6, .next = NULL };
        my_list list = { .first = NULL, .last = NULL };

        assert(pmt_ll_push_back(&my_list_iface, &list, &n1) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n2) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n3) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n4) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n5) == &list);
        assert(pmt_ll_push_back(&my_list_iface, &list, &n6) == &list);
        assert(match_list(list.first, (int[]){ 1, 2, 3, 4, 5, 6 }, 6));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 6);
        
        assert(pmt_ll_reverse(&my_list_iface, &list) == &list);
        assert(match_list(list.first, (int[]){ 6, 5, 4, 3, 2, 1 }, 6));
        assert(pmt_ll_node_count(&my_node_iface, list.first) == 6);
        assert(list.first == &n6 && list.last == &n1);
}

int main(int argc, char **args) 
{
        puts("testing - linked_list.c");

        test_node_push_front();
        test_node_last();
        test_node_count();
        test_node_at();
        test_node_push_back();
        test_node_insert_after();
        test_node_remove_after();
        test_node_remove_first();
        test_node_remove_last();
        test_node_remove_when();
        test_node_filter();
        test_node_find();
        test_node_foreach();
        test_node_reverse();

        test_push_front();
        test_push_back();
        test_insert_after();
        test_remove_after();
        test_remove_first();
        test_remove_last();
        test_remove_when();
        test_filter();
        test_reverse();

        return 0;
}