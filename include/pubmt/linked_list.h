#ifndef PUBMT_LINKED_LIST_H
#define PUBMT_LINKED_LIST_H

#include <stddef.h>
#include <stdbool.h>

/** Linked List Node Interface */
typedef struct pmt_ll_node_iface {
        void *(*get_next)(void *node);
        void (*set_next)(void *node, void *next);
} pmt_ll_node_iface;

/** 
 * Traverses the list to find the last node O(n). 
 * 
 * RETURNS: The last node or NULL if list is NULL.
 */
void *pmt_ll_node_last(pmt_ll_node_iface *iface, void *list);

/** 
 * Count total nodes in the list O(n). 
 * 
 * RETURNS: The number of nodes in the list.
 */
size_t pmt_ll_node_count(pmt_ll_node_iface *iface, void *list);

/** 
 * Get node at index O(n).
 * 
 * RETURNS: The node at the given index or NULL if scan went out of bounds.
 */
void *pmt_ll_node_at(pmt_ll_node_iface *iface, void *list, const size_t index);

/** 
 * Push 'fst', an unlinked node, onto the front of the list. 
 * 
 * RETURNS: fst
 */
void *pmt_ll_node_push_front(pmt_ll_node_iface *iface, void *list, void *fst);

/** 
 * Push 'last' onto the back of the list O(n).  If 'last' is not a singleton,
 * this function will concatenate the two lists.
 * 
 * RETURNS: 
 *      list - List was not empty.
 *      last - List was NULL.
 */
void *pmt_ll_node_push_back(pmt_ll_node_iface *iface, void *list, void *last);

/** 
 * Inserts 'succ', an unlinked node, immediately after node O(1). 
 * 
 * RETURNS: node
 */
void *pmt_ll_node_insert_after(
        pmt_ll_node_iface *iface, 
        void *node, 
        void *succ);

/** 
 * Remove the node's successor O(1).
 * 
 * RETURNS: The removed successor, or NULL if there was none.
 */
void *pmt_ll_node_remove_after(pmt_ll_node_iface *iface, void *node);

/** 
 * Remove the first node of the list O(1).
 * 
 * RETURNS: The removed node, or NULL if the list was empty.
 * MODIFIES: The value of 'list' is updated to reflect any changes.
 */
void *pmt_ll_node_remove_first(pmt_ll_node_iface *iface, void **list);

/**
 * Remove the last node of the list O(n). 
 * 
 * RETURNS: The removed node, or NULL if list was empty.
 * MODIFIES: 
 *      The value of 'list_ref' points to the beginning of the modified list.
 *      The value of 'pred_ref' is the deleted node's predecessor.
 */
void *pmt_ll_node_remove_last_args(
        pmt_ll_node_iface *iface, 
        void **list_ref,
        void **pred_ref);

/**
 * Remove the last node of the list O(n). 
 * 
 * RETURNS: The removed node, or NULL if list was empty.
 * MODIFIES: 
 *      The value of 'list' points to the beginning of the modified list.
 */
void *pmt_ll_node_remove_last(pmt_ll_node_iface *iface, void **list);

/**
 * Remove the first node that matches the predicate O(n).
 * 
 * RETURNS: The removed node, or NULL if not found.
 * MODIFIES: 
 *      The value of 'list_ref' points to the beginning of the modified list.
 *      The value of 'pred_ref' is the deleted node's predecessor.
 * 
 */
void *pmt_ll_node_remove_when_args(
        pmt_ll_node_iface *iface, 
        void **list_ref, 
        bool (*predicate)(void *node, void *state),
        void *state,
        void **pred_ref);

/**
 * Remove the first node that matches the predicate O(n).
 * 
 * RETURNS: The removed node, or NULL if not found.
 * MODIFIES: 
 *      The value of 'list' points to the beginning of the modified list.
 */
void *pmt_ll_node_remove_when(
        pmt_ll_node_iface *iface, 
        void **list, 
        bool (*predicate)(void *node, void *state),
        void *state);


/** 
 * Filter list, removing any nodes that do not match the predicate O(n). 
 * 
 * RETURNS: The number of elements removed.
 * MODIFIES: 
 *      The value of 'list_ref' points to the beginning of the modified list.
 *      The value of 'last_ref' points to the last node of the modified list.
 */
size_t pmt_ll_node_filter_args(
        pmt_ll_node_iface *iface, 
        void **first_ref, 
        bool (*predicate)(void *node, void *state),
        void *state,
        void **last_ref);

/** 
 * Filter list, removing any nodes that do not match the predicate O(n). 
 * 
 * RETURNS: The number of elements removed.
 * MODIFIES: The value of 'list' points to the beginning of the modified list.
 */
size_t pmt_ll_node_filter(
        pmt_ll_node_iface *iface, 
        void **list, 
        bool (*predicate)(void *node, void *state),
        void *state);

/** 
 * Find first node in the list that matches the predicate O(n). 
 * 
 * RETURNS: The first node matching the predicate, or NULL if none were found.
 */
void *pmt_ll_node_find(
        pmt_ll_node_iface *iface, 
        void *list,
        bool (*predicate)(void *node, void *state),
        void *state);

/** 
 * Iterate through the list, calling the function for each node O(n).  This 
 * function will return early if the callback returns false.
 * 
 * RETURNS: Returns true if the iteration completed.
 */
bool pmt_ll_node_foreach(
        pmt_ll_node_iface *iface, 
        void *list, 
        bool (*callback)(void *node, void *state),
        void *state);

/**
 * Reverse the list in place O(n).
 * 
 * RETURNS: The reversed list.
 */
void *pmt_ll_node_reverse(pmt_ll_node_iface *iface, void *list);

/** Singly Linked List Interface */
typedef struct pmt_ll_iface {
        pmt_ll_node_iface node_iface;
        void *(*get_first)(void *list);
        void (*set_first)(void *list, void *first);
        void *(*get_last)(void *list);
        void (*set_last)(void *list, void *last);
} pmt_ll_iface;

/**
 * Is the list empty O(1). 
 * 
 * RETURNS: true if empty, otherwise false.
 */
bool pmt_ll_is_empty(pmt_ll_iface *iface, void *list);

/** 
 * Push 'first', an unlinked node, onto the front of the list O(1). 
 * 
 * RETURNS: list
 */
void *pmt_ll_push_front(pmt_ll_iface *iface, void *list, void *first);

/** 
 * Push the node 'last' onto the back of the list.  Complexity is O(1) in 
 * the case where 'last' is a singleton.  
 * 
 * RETURNS: list
 */
void *pmt_ll_push_back(pmt_ll_iface *iface, void *list, void *last);

/** 
 * Inserts succ, an unlinked node, immediately after node O(1). 
 * 
 * RETURNS: list
 */
void *pmt_ll_insert_after(
        pmt_ll_iface *iface, 
        void *list, 
        void *node,
        void *succ);

/** 
 * Remove the node's successor O(1).
 * 
 * RETURNS: The removed successor, or NULL if there was none.
 */
void *pmt_ll_remove_after(pmt_ll_iface *iface, void *list, void *node);

/** 
 * Remove the first node of the list O(1).
 * 
 * RETURNS: The removed node, or NULL if the list was empty.
 */
void *pmt_ll_remove_first(pmt_ll_iface *iface, void *list);

/**
 * Remove the last node of the list O(n). 
 * 
 * RETURNS: The removed node, or NULL if the list was empty.
 */
void *pmt_ll_remove_last(pmt_ll_iface *iface, void *list);

/**
 * Remove the first node that matches the predicate O(n).
 * 
 * RETURNS: The removed node, or NULL if not found.
 */
void *pmt_ll_remove_when(
        pmt_ll_iface *iface, 
        void *list, 
        bool (*predicate)(void *node, void *state),
        void *state);

/** 
 * Filter the list, removing any nodes that do not match the predicate O(n). 
 * 
 * RETURNS: The number of elements removed.
 */
size_t pmt_ll_filter(
        pmt_ll_iface *iface, 
        void *list, 
        bool (*predicate)(void *node, void *state),
        void *state);

/**
 * Reverse the list in place O(n).
 * 
 * RETURNS: The reversed list.
 */
void *pmt_ll_reverse(pmt_ll_iface *iface, void *list);

#endif
