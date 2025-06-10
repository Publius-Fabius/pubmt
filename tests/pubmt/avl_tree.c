
#include "pubmt/avl_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct my_node {

        int key, height;

        struct my_node *right, *left;
        
} my_node_t;

typedef struct my_tree {

        my_node_t *root;

} my_tree_t;

void *get_left(void *node)
{
        return ((my_node_t*)node)->left;
}

void set_left(void *node, void *left)
{
        ((my_node_t*)node)->left = left;
}

void *get_right(void *node)
{
        return ((my_node_t*)node)->right;
}

void set_right(void *node, void *right)
{
        ((my_node_t*)node)->right = right;
}

void *get_key(void *node)
{
        return &((my_node_t*)node)->key;
}

int get_height(void *node)
{
        return ((my_node_t*)node)->height;
}

void set_height(void *node, const int height)
{
        ((my_node_t*)node)->height = height;
}

bool less_than(void *key_a, void *key_b)
{
        return *((int*)key_a) < *((int*)key_b);
}

pmt_avl_less_than_t get_less_than(void *tree)
{
        return less_than;
}

void *get_root(void *tree)
{
        return ((my_tree_t*)tree)->root;
}

void set_root(void *tree, void *root)
{
        ((my_tree_t*)tree)->root = root;
}

pmt_avl_iface_t my_iface = {
        .node_iface = {
                .get_left = get_left,
                .set_left = set_left,
                .get_right = get_right,
                .set_right = set_right,
                .get_key = get_key,
                .get_height = get_height,
                .set_height = set_height },
        .get_less_than = get_less_than,
        .get_root = get_root,
        .set_root = set_root
};

int shuffle_cmp(const void *a, const void *b)
{
        return (rand() % 3) - 1; 
}

void shuffle(my_node_t *nodes, int count)
{
        qsort(nodes, (size_t)count, sizeof(my_node_t), shuffle_cmp);
}

my_node_t *make_deck(const int count)
{
        my_node_t *deck = malloc(sizeof(my_node_t) * (size_t)count);

        for(int x = 0; x < count; ++x){
                deck[x].key = x;
                deck[x].height = 0;
                deck[x].left = NULL;
                deck[x].right = NULL;
        }
        
        shuffle(deck, count);
        
        return deck;
}

void test_insert_only()
{
        my_tree_t tree = { .root = NULL };
        pmt_avl_stack_t stack;
        my_node_t *deck = make_deck(500);

        printf("test_insert_only(): sample deck: ");
        for(int x = 0; x < 20; ++x) {
                printf("%i ", deck[x].key); 
        }
        printf("\n");

        for(int x = 0; x < 500; ++x) {
                assert(pmt_avl_insert(&my_iface, &tree, &stack, deck + x));
                my_node_t dummy = {
                        .height = 0,
                        .key = deck[x].key,
                        .left = NULL,
                        .right = NULL };
                assert(!pmt_avl_insert(&my_iface, &tree, &stack, &dummy));
        }

        free(deck);
}

void test_insert_iterate()
{
        my_tree_t tree = { .root = NULL };
        pmt_avl_stack_t stack;
        my_node_t *deck = make_deck(500);

        for(int x = 0; x < 500; ++x) {
                assert(pmt_avl_insert(&my_iface, &tree, &stack, deck + x));
                my_node_t dummy = {
                        .height = 0,
                        .key = deck[x].key,
                        .left = NULL,
                        .right = NULL };
                assert(!pmt_avl_insert(&my_iface, &tree, &stack, &dummy));
        }

        pmt_avl_entries(&my_iface, &tree, &stack);

        void *node = NULL;
        int count = 0;

        while(pmt_avl_next(&my_iface, &stack, &node)) {
                assert(node);
                my_node_t *n = node;
                assert(n->key == count++);
        }

        assert(count == 500);

        pmt_avl_reversed(&my_iface, &tree, &stack);

        while(pmt_avl_prior(&my_iface, &stack, &node)) {
                assert(node);
                my_node_t *n = node;
                assert(n->key == --count);
        }

        assert(count == 0);

        free(deck);
}

void test_insert_lookup()
{
        my_tree_t tree = { .root = NULL };
        pmt_avl_stack_t stack;
        my_node_t *deck = make_deck(500);

        for(int x = 0; x < 500; ++x) {
                assert(pmt_avl_insert(&my_iface, &tree, &stack, deck + x));
                my_node_t dummy = {
                        .height = 0,
                        .key = deck[x].key,
                        .left = NULL,
                        .right = NULL };
                assert(!pmt_avl_insert(&my_iface, &tree, &stack, &dummy));
        }

        for(int x = 0; x < 500; ++x) {
                int key = deck[x].key;
                assert(pmt_avl_lookup(&my_iface, &tree, &key) == deck + x);
        }

        free(deck);
}

void test_min_max()
{
        my_tree_t tree = { .root = NULL };
        pmt_avl_stack_t stack;
        my_node_t *deck = make_deck(500);

        assert(!pmt_avl_min(&my_iface, &tree));
        assert(!pmt_avl_max(&my_iface, &tree));

        for(int x = 0; x < 500; ++x) {
                assert(pmt_avl_insert(&my_iface, &tree, &stack, deck + x));
                my_node_t dummy = {
                        .height = 0,
                        .key = deck[x].key,
                        .left = NULL,
                        .right = NULL };
                assert(!pmt_avl_insert(&my_iface, &tree, &stack, &dummy));
        }

        assert(((my_node_t*)pmt_avl_min(&my_iface, &tree))->key == 0);
        assert(((my_node_t*)pmt_avl_max(&my_iface, &tree))->key == 499);

        free(deck);
}

void test_remove()
{
        my_tree_t tree = { .root = NULL };
        pmt_avl_stack_t stack;
        my_node_t *deck = make_deck(500);

        for(int x = 0; x < 500; ++x) {
                assert(pmt_avl_insert(&my_iface, &tree, &stack, deck + x));
                my_node_t dummy = {
                        .height = 0,
                        .key = deck[x].key,
                        .left = NULL,
                        .right = NULL };
                assert(!pmt_avl_insert(&my_iface, &tree, &stack, &dummy));
        }

        for(int x = 0; x < 500; ++x) {
                int key = deck[x].key;
                assert(pmt_avl_remove(
                        &my_iface, &tree, &stack, &key) == deck + x);
                assert(!pmt_avl_remove(&my_iface, &tree, &stack, &key));
        }
       
        assert(pmt_avl_is_empty(&my_iface, &tree));

        free(deck);
}

void test_remove_min()
{
        my_tree_t tree = { .root = NULL };
        pmt_avl_stack_t stack;
        my_node_t *deck = make_deck(500);

        for(int x = 0; x < 500; ++x) {
                assert(pmt_avl_insert(&my_iface, &tree, &stack, deck + x));
                my_node_t dummy = {
                        .height = 0,
                        .key = deck[x].key,
                        .left = NULL,
                        .right = NULL };
                assert(!pmt_avl_insert(&my_iface, &tree, &stack, &dummy));
        }

        for(int x = 0; x < 500; ++x) {
                assert(((my_node_t*)pmt_avl_remove_min(
                        &my_iface, &tree, &stack))->key == x);
        }

        assert(pmt_avl_is_empty(&my_iface, &tree));
       
        free(deck);
}

void test_remove_max()
{
        my_tree_t tree = { .root = NULL };
        pmt_avl_stack_t stack;
        my_node_t *deck = make_deck(500);

        for(int x = 0; x < 500; ++x) {
                assert(pmt_avl_insert(&my_iface, &tree, &stack, deck + x));
                my_node_t dummy = {
                        .height = 0,
                        .key = deck[x].key,
                        .left = NULL,
                        .right = NULL };
                assert(!pmt_avl_insert(&my_iface, &tree, &stack, &dummy));
        }

        for(int x = 0; x < 500; ++x) {
                assert(((my_node_t*)pmt_avl_remove_max(
                        &my_iface, &tree, &stack))->key == (499 - x));
        }

        assert(pmt_avl_is_empty(&my_iface, &tree));
       
        free(deck);
}

void test_upper()
{
        my_tree_t tree = { .root = NULL };
        pmt_avl_stack_t stack;
        my_node_t *deck = make_deck(500);

        for(int x = 0; x < 500; ++x) {
                assert(pmt_avl_insert(&my_iface, &tree, &stack, deck + x));
                my_node_t dummy = {
                        .height = 0,
                        .key = deck[x].key,
                        .left = NULL,
                        .right = NULL };
                assert(!pmt_avl_insert(&my_iface, &tree, &stack, &dummy));
        }

        int key = 250, count = 250;
        assert(pmt_avl_upper(&my_iface, &tree, &stack, &key));

        void *node = NULL;
        while(pmt_avl_next(&my_iface, &stack, &node)) {
                assert(node);
                my_node_t *n = node;
                assert(n->key == count++);
        }

        assert(count == 500);
       
        free(deck);
}

void test_lower()
{
        my_tree_t tree = { .root = NULL };
        pmt_avl_stack_t stack;
        my_node_t *deck = make_deck(500);

        for(int x = 0; x < 500; ++x) {
                assert(pmt_avl_insert(&my_iface, &tree, &stack, deck + x));
                my_node_t dummy = {
                        .height = 0,
                        .key = deck[x].key,
                        .left = NULL,
                        .right = NULL };
                assert(!pmt_avl_insert(&my_iface, &tree, &stack, &dummy));
        }

        int key = 250, count = 251;
        assert(pmt_avl_lower(&my_iface, &tree, &stack, &key));

        void *node = NULL;
        while(pmt_avl_prior(&my_iface, &stack, &node)) {
                assert(node);
                my_node_t *n = node;
                assert(n->key == --count);
        }

        assert(count == 0);
       
        free(deck);
}

int main(int argc, char **args)
{
        puts("testing -- avl_tree.c");

        test_insert_only();
        test_insert_iterate();
        test_insert_lookup();
        test_min_max();
        test_remove();
        test_remove_min();
        test_remove_max();
        test_upper();
        test_lower();
}