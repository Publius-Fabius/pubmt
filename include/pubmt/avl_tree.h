#ifndef PUBMT_AVL_TREE_H
#define PUBMT_AVL_TREE_H

#include <stddef.h>
#include <stdbool.h>

/** Is key_a less than key_b? */
typedef bool (*pmt_avl_less_than_t)(void *key_a, void *key_b);

/** AVL Tree Callback Interface */
typedef struct pmt_avl_iface {

        void *(*get_left)(void *node);
        void *(*get_right)(void *node);

        void *(*get_key)(void *node);

        size_t get_height(void *node);
        void set_height(void *node, const size_t height);

        pmt_avl_less_than_t (*get_less_than)(void *tree);

        size_t get_size(void *tree);
        void set_size(void *tree, const size_t size);

} pmt_avl_iface_t;

/** AVL Tree Iterator */
typedef struct pmt_avl_stack {

        void *nodes[64];

        size_t offset;

} pmt_avl_stack_t;

/**
 * Reset the stack.
 */
void pmt_avl_reset(pmt_avl_stack_t *iter);

/**
 * Initialize an AVL tree.
 */
void pmt_avl_init(pmt_avl_iface_t *iface, void *tree);

/**
 * Insert a node.
 */
bool pmt_avl_insert(
        pmt_avl_iface_t *iface, 
        void *tree, 
        pmt_avl_stack_t *stack,
        void *node);

/**
 * Look up a node.
 */
void *pmt_avl_lookup(pmt_avl_iface_t *iface, void *tree, void *key);

/**
 * Get minimum node.
 */
void *pmt_avl_min(pmt_avl_iface_t *iface,  void *tree);

/**
 * Get maximum node.
 */
void *pmt_avl_max(pmt_avl_iface_t *iface,  void *tree);

/**
 * Remove a node.
 */
void *pmt_avl_remove(
        pmt_avl_iface_t *iface, 
        void *tree, 
        pmt_avl_stack *stack,
        void *key);

/** 
 * Remove the minimum node.
 */
void *pmt_avl_remove_min(
        pmt_avl_iface_t *iface,  
        void *tree,
        pmt_avl_stack_t *stack);

/**
 * Remove the maximum node.
 */
void *pmt_avl_remove_max(
        pmt_avl_iface_t *iface,  
        void *tree,
        pmt_avl_stack_t *stack);

/** 
 * Iteration in ascending order.
 */
pmt_avl_stack_t *pmt_avl_entries(
        pmt_avl_iface_t *iface, 
        void *tree, 
        pmt_avl_stack_t *iter);

 /** 
 * Iteration in descending order.
 */
pmt_avl_stack_t *pmt_avl_reversed(
        pmt_avl_iface_t *iface, 
        void *tree, 
        pmt_avl_stack_t *iter);

/** 
 * Ascending iteration from entries equal to or greater than the key. 
 */
void *pmt_avl_upper(
        pmt_avl_iface_t *iface, 
        void *tree,
        pmt_avl_stack_t *stack,
        void *key);

/** 
 * Descending iteration from entries equal to or less than the key.
 */
void *pmt_avl_lower(
        pmt_avl_iface_t *iface,  
        void *tree,
        pmt_avl_stack_t *stack,
        void *key);

/**
 * Get the next node in ascending order.
 */
bool pmt_avl_next(pmt_avl_iface_t *iface, pmt_avl_stack_t *stack, void **node);

/**
 * Get the next node in descending order.
 */
bool pmt_avl_prior(pmt_avl_iface_t *iface, pmt_avl_stack_t *stack, void **node);

#endif