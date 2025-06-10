#ifndef PUBMT_AVL_TREE_H
#define PUBMT_AVL_TREE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/** Is key_a less than key_b? */
typedef bool (*pmt_avl_less_than_t)(void *key_a, void *key_b);

/** AVL Tree Node Callback Interface */
typedef struct pmt_avl_node_iface {

        void *(*get_left)(void *node);
        void (*set_left)(void *node, void *left);

        void *(*get_right)(void *node);
        void (*set_right)(void *node, void *right);

        void *(*get_key)(void *node);

        int (*get_height)(void *node);
        void (*set_height)(void *node, const int height);

} pmt_avl_node_iface_t;

/** AVL Tree Callback Interface */
typedef struct pmt_avl_iface {

        pmt_avl_node_iface_t node_iface;

        pmt_avl_less_than_t (*get_less_than)(void *tree);

        void *(*get_root)(void *tree);

        void (*set_root)(void *tree, void *root);

} pmt_avl_iface_t;

#if UINTPTR_MAX > 4294967295UL

        #define PMT_AVL_STACK_MAX 64

#elif UINTPTR_MAX > 65535U
        
        #define PMT_AVL_STACK_MAX 32

#else 

        #define PMT_AVL_STACK_MAX 16

#endif

/** AVL Tree Stack */
typedef struct pmt_avl_stack {

        void *nodes[PMT_AVL_STACK_MAX];

        int offset;

} pmt_avl_stack_t;

/**
 * Validate the AVL callback interface. 
 * 
 * @returns Will return 'false' if any callbacks are NULL.
 */
bool pmt_avl_iface_validate(pmt_avl_iface_t *iface);

/**
 * Reset the stack.
 */
void pmt_avl_stack_reset(pmt_avl_stack_t *stack);

/**
 * Initialize an AVL tree.
 */
void *pmt_avl_init(pmt_avl_iface_t *iface, void *tree);

/**
 * Is the tree empty?
 * 
 * @returns A return value of 'true' indicates that the tree is empty.
 */
bool pmt_avl_is_empty(pmt_avl_iface_t *iface, void *tree);

/**
 * Insert a node into the tree.
 * 
 * @returns A value of 'false' indicates another node with the same key 
 * already exists within the tree.
 */
bool pmt_avl_insert(
        pmt_avl_iface_t *iface, 
        void *tree, 
        pmt_avl_stack_t *stack,
        void *node);

/**
 * Look for the node with the given key.
 * 
 * @returns A pointer to a node within the tree, or NULL if no node exists
 * within the tree with the given key.
 */
void *pmt_avl_lookup(pmt_avl_iface_t *iface, void *tree, void *key);

/**
 * Get the minimum node.
 * 
 * @returns The minimum node, or NULL if the tree is empty.
 */
void *pmt_avl_min(pmt_avl_iface_t *iface,  void *tree);

/**
 * Get the maximum node.
 * 
 * @returns The maximum node, or NULL if the tree is empty.
 */
void *pmt_avl_max(pmt_avl_iface_t *iface,  void *tree);

/**
 * Remove the node with the given key.
 * 
 * @returns A pointer to the removed node is returned.  If no node with the
 * given key is found, NULL is returned instead.
 */
void *pmt_avl_remove(
        pmt_avl_iface_t *iface, 
        void *tree, 
        pmt_avl_stack_t *stack,
        void *key);

/** 
 * Remove the minimum node.
 * 
 * @returns A pointer to the minimum node is returned, or NULL if the tree is 
 * empty.
 */
void *pmt_avl_remove_min(
        pmt_avl_iface_t *iface,  
        void *tree,
        pmt_avl_stack_t *stack);

/** 
 * Remove the maximum node.
 * 
 * @returns A pointer to the maximum node is returned, or NULL if the tree is 
 * empty.
 */
void *pmt_avl_remove_max(
        pmt_avl_iface_t *iface,  
        void *tree,
        pmt_avl_stack_t *stack);

/** 
 * Setup an iteration in ascending order.
 */
pmt_avl_stack_t *pmt_avl_entries(
        pmt_avl_iface_t *iface, 
        void *tree, 
        pmt_avl_stack_t *stack);

 /** 
 * Setup an iteration in descending order.
 */
pmt_avl_stack_t *pmt_avl_reversed(
        pmt_avl_iface_t *iface, 
        void *tree, 
        pmt_avl_stack_t *stack);

/**
 * Get the next node in ascending order.  This function can only be used 
 * with a stack setup for ascending traversal, any other usage is
 * undefined.
 * 
 * @returns A value of 'false' is returned when the iteration is complete.
 */
bool pmt_avl_next(
        pmt_avl_iface_t *iface, 
        pmt_avl_stack_t *stack, 
        void **node);

/**
 * Get the next node in descending order.  This function can only be used 
 * with a stack setup for descending traversal, any other usage is
 * undefined.
 * 
 * @returns A value of 'false' is returned when the iteration is complete.
 */
bool pmt_avl_prior(
        pmt_avl_iface_t *iface, 
        pmt_avl_stack_t *stack, 
        void **node);

/** 
 * Setup an ascending iteration from entries equal to or greater than the key. 
 */
pmt_avl_stack_t *pmt_avl_upper(
        pmt_avl_iface_t *iface, 
        void *tree,
        pmt_avl_stack_t *stack,
        void *key);

/** 
* Setup a descending iteration from entries equal to or less than the key. 
*/
pmt_avl_stack_t *pmt_avl_lower(
        pmt_avl_iface_t *iface,  
        void *tree,
        pmt_avl_stack_t *stack,
        void *key);

#endif