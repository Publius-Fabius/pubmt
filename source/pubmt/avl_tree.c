
#include "pubmt/avl_tree.h"
#include <assert.h>

static inline int pmt_avl_node_height(
        pmt_avl_node_iface_t *iface, 
        void *node)
{
        return node ? iface->get_height(node) : 0;
}

static inline void pmt_avl_node_update_height(
        pmt_avl_node_iface_t *iface, 
        void *node)
{
        const int 
                h_l = pmt_avl_node_height(iface, iface->get_left(node)),
                h_r = pmt_avl_node_height(iface, iface->get_right(node));

        iface->set_height(node, 1 + (h_l < h_r ? h_r : h_l));
}

static inline void *pmt_avl_rotate_left(
        pmt_avl_node_iface_t *iface, 
        void *node)
{
        assert(node);
        void *y = iface->get_right(node);
        assert(y);
        void *t = iface->get_left(y);
        
        iface->set_left(y, node);
        iface->set_right(node, t);

        pmt_avl_node_update_height(iface, node);
        pmt_avl_node_update_height(iface, y);

        return y;
}

static inline void *pmt_avl_rotate_right(
        pmt_avl_node_iface_t *iface, 
        void *node)
{
        assert(node);

        void *x = iface->get_left(node);
        assert(x);
        void *t = iface->get_right(x);

        iface->set_right(x, node);
        iface->set_left(node, t);

        pmt_avl_node_update_height(iface, node);
        pmt_avl_node_update_height(iface, x);

        return x;
}

static inline int pmt_avl_node_balance_factor(
        pmt_avl_node_iface_t *iface, 
        void *node) 
{
        assert(node);

        const int 
                h_r = pmt_avl_node_height(iface, iface->get_right(node)),
                h_l = pmt_avl_node_height(iface, iface->get_left(node));

        return h_r - h_l;
}

static void *pmt_avl_node_rebalance(
        pmt_avl_node_iface_t *iface, 
        void *node)
{
        pmt_avl_node_update_height(iface, node);

        const int balance = pmt_avl_node_balance_factor(iface, node);

        assert(-3 < balance && balance < 3);

        if(balance > 1) {

                void *right = iface->get_right(node);
                assert(right);

                if(pmt_avl_node_balance_factor(iface, right) < 0) {
                        right = pmt_avl_rotate_right(iface, right);
                        assert(right);
                        iface->set_right(node, right);
                } 

                return pmt_avl_rotate_left(iface, node);

        } else if(balance < -1) {

                void *left = iface->get_left(node);
                assert(left);

                if(pmt_avl_node_balance_factor(iface, left) > 0) {
                        left = pmt_avl_rotate_left(iface, left);
                        assert(left);
                        iface->set_left(node, left);
                } 

                return pmt_avl_rotate_right(iface, node);
        } 

        return node;
}

static inline bool pmt_avl_stack_push(pmt_avl_stack_t *stack, void *node)
{
        assert(stack && stack->offset <= PMT_AVL_STACK_MAX);

        if(stack->offset >= PMT_AVL_STACK_MAX) {
                return false;
        }
        
        stack->nodes[stack->offset++] = node;

        return true;
}

static inline void *pmt_avl_stack_pop(pmt_avl_stack_t *stack)
{
        assert(stack && stack->offset <= PMT_AVL_STACK_MAX);

        if(stack->offset == 0) {
                return NULL;
        }

        return stack->nodes[--stack->offset];
}

void pmt_avl_stack_reset(pmt_avl_stack_t *stack)
{
        assert(stack);

        stack->offset = 0;
}

static void *pmt_avl_stack_rebalance(
        pmt_avl_node_iface_t *iface,
        pmt_avl_stack_t *stack,
        const int nsteps, 
        void *top)
{
        assert(top && nsteps <= stack->offset);
        
        void *new_top = pmt_avl_node_rebalance(iface, top);
        
        for(int i = 0; i < nsteps; ++i) {

                void *next = pmt_avl_stack_pop(stack);

                assert(new_top && next);

                if(iface->get_left(next) == top) {
                        iface->set_left(next, new_top);
                } else if(iface->get_right(next) == top) {
                        iface->set_right(next, new_top);
                } else {
                        assert(0 /* Top not found in parent node. */); 
                }

                top = next;
                new_top = pmt_avl_node_rebalance(iface, next);
        }

        return new_top;
}

bool pmt_avl_iface_validate(pmt_avl_iface_t *iface)
{
        return 
                iface && 
                iface->node_iface.get_left &&
                iface->node_iface.set_left &&
                iface->node_iface.get_right &&
                iface->node_iface.set_right &&
                iface->node_iface.get_key && 
                iface->node_iface.get_height &&
                iface->node_iface.set_height &&
                iface->get_less_than &&
                iface->get_root &&
                iface->set_root;
}

void *pmt_avl_init(pmt_avl_iface_t *iface, void *tree)
{
        assert(tree && pmt_avl_iface_validate(iface));
        iface->set_root(tree, NULL);
        return tree;
}

bool pmt_avl_is_empty(pmt_avl_iface_t *iface, void *tree)
{
        assert(tree && pmt_avl_iface_validate(iface));
        return iface->get_root(tree) == NULL;
}

bool pmt_avl_insert(
        pmt_avl_iface_t *iface,
        void *tree,
        pmt_avl_stack_t *stack,
        void *node)
{
        assert(tree && pmt_avl_iface_validate(iface));

        pmt_avl_node_iface_t *node_iface = &iface->node_iface;

        assert(!node_iface->get_left(node) && !node_iface->get_right(node));

        void *root = iface->get_root(tree);

        if(!root) {
                iface->set_root(tree, node);
                return true;
        }

        pmt_avl_less_than_t less_than = iface->get_less_than(tree);

        void 
                *key = node_iface->get_key(node),
                *search = root;

        int side = 0;

        pmt_avl_stack_reset(stack);

        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {

                if(!search) {
                        goto FINISH_UP;
                } 
                
                void *search_key = node_iface->get_key(search);

                if(less_than(key, search_key)) {
                        if(!pmt_avl_stack_push(stack, search)) {
                                assert(0);
                                return false;
                        }
                        search = node_iface->get_left(search);
                        side = -1;
                } else if(less_than(search_key, key)) {
                        if(!pmt_avl_stack_push(stack, search)) {
                                assert(0);
                                return false;
                        }
                        search = node_iface->get_right(search);
                        side = 1;
                } else {
                        return false;
                }
        }

        assert(0);
        return false;

        FINISH_UP:;

        void *top = pmt_avl_stack_pop(stack);

        assert(top && side);

        if(side < 0) {
                node_iface->set_left(top, node);
        } else {
                node_iface->set_right(top, node);
        }

        iface->set_root(tree, pmt_avl_stack_rebalance(
                node_iface, 
                stack, 
                stack->offset, 
                top));

        return true;
}

void *pmt_avl_lookup(pmt_avl_iface_t *iface, void *tree, void *key)
{
        assert(tree && key && pmt_avl_iface_validate(iface));

        pmt_avl_less_than_t less_than = iface->get_less_than(tree);

        void *search = iface->get_root(tree);

        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {
                if(!search) {
                        return NULL;
                } 
                void *search_key = iface->node_iface.get_key(search);
                if(less_than(key, search_key)) {
                        search = iface->node_iface.get_left(search);
                } else if(less_than(search_key, key)) {
                        search = iface->node_iface.get_right(search);
                } else {
                        return search;
                }
        }

        assert(0);
        return NULL;
}

static void *pmt_avl_node_remove_entry_nary(
        pmt_avl_node_iface_t *iface,
        pmt_avl_stack_t *stack,
        void *const left,
        void *const right,
        void *right_left)
{
        const int saved_offset = stack->offset;

        if(!pmt_avl_stack_push(stack, right)) {
                assert(0);
                return NULL;
        } else if(!pmt_avl_stack_push(stack, right_left)) {
                assert(0);
                return NULL;
        }

        right_left = iface->get_left(right_left);

        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {
                if(!right_left) {
                        goto FINISH_UP;
                } else {
                        if(!pmt_avl_stack_push(stack, right_left)) {
                                assert(0);
                                return NULL;
                        }
                        right_left = iface->get_left(right_left);
                }
        }

        assert(0);
        return NULL;

        FINISH_UP:;

        void    *top = NULL,
                *succ = NULL;

        if(!(succ = pmt_avl_stack_pop(stack))) {
                assert(0);
                return NULL;
        } else if(!(top = pmt_avl_stack_pop(stack))) {
                assert(0);
                return NULL;
        }

        assert(stack->offset >= saved_offset);

        /* 
                Because top.left = succ, this step will delink succ. 
        */
        iface->set_left(top, iface->get_right(succ));

        /* 
                Rebalance { deleted_node.right, ..., top } and set the result 
                to succ.right. 
        */
        iface->set_right(succ, pmt_avl_stack_rebalance(
                iface,
                stack,
                stack->offset - saved_offset,
                top));

        /* 
                Set succ.left to deleted_node.left.
        */
        iface->set_left(succ, left);

        /*
                The result of rebalancing 'succ' can be patched back in.
        */
        return pmt_avl_node_rebalance(iface, succ);
}

static void *pmt_avl_node_remove_entry(
        pmt_avl_node_iface_t *iface,
        pmt_avl_stack_t *stack,
        void *const entry)
{
        void 
                *patch = NULL,
                *left = iface->get_left(entry),
                *right = iface->get_right(entry),
                *right_left = NULL;

        if(!left) {
                patch = right;
        } else if(!right) {
                patch = left;
        } else if(!(right_left = iface->get_left(right))) {
                iface->set_left(right, left);
                patch = pmt_avl_node_rebalance(iface, right);
        } else {
                patch =  pmt_avl_node_remove_entry_nary(
                        iface,
                        stack,
                        left,
                        right,
                        right_left);
        }

        if(!stack->offset) {
                return patch;
        }

        void *top = pmt_avl_stack_pop(stack);

        if(iface->get_left(top) == entry) {
                iface->set_left(top, patch);
        } else if(iface->get_right(top) == entry) {
                iface->set_right(top, patch);
        } else {
                assert(0);
                return NULL;
        }

        return pmt_avl_stack_rebalance(
                iface,
                stack,
                stack->offset,
                top);
}

void *pmt_avl_remove(
        pmt_avl_iface_t *iface, 
        void *tree, 
        pmt_avl_stack_t *stack,
        void *key)
{
        assert(tree && key && pmt_avl_iface_validate(iface));

        pmt_avl_node_iface_t *node_iface = &iface->node_iface;
        pmt_avl_less_than_t less_than = iface->get_less_than(tree);

        void *search = iface->get_root(tree);

        pmt_avl_stack_reset(stack);

        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {

                if(!search) {
                        return NULL;
                } 
                
                void *search_key = node_iface->get_key(search);

                if(less_than(key, search_key)) {
                        if(!pmt_avl_stack_push(stack, search)) {
                                assert(0);
                                return NULL;
                        }
                        search = node_iface->get_left(search);
                } else if(less_than(search_key, key)) {
                        if(!pmt_avl_stack_push(stack, search)) {
                                assert(0);
                                return NULL;
                        }
                        search = node_iface->get_right(search);
                } else {
                        iface->set_root(tree, pmt_avl_node_remove_entry(
                                node_iface,
                                stack,
                                search));
                        node_iface->set_left(search, NULL);
                        node_iface->set_right(search, NULL);
                        return search;
                }
        }

        assert(0);
        return NULL;
}

void *pmt_avl_remove_min(
        pmt_avl_iface_t *iface,  
        void *tree,
        pmt_avl_stack_t *stack)
{
        assert(tree && pmt_avl_iface_validate(iface));

        void *search = iface->get_root(tree);

        if(!search) {
                return NULL;
        }

        pmt_avl_stack_reset(stack);

        if(!pmt_avl_stack_push(stack, search)) {
                assert(0);
                return NULL;
        }

        search = iface->node_iface.get_left(search);
        
        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {
                if(!search) {
                        void *entry = pmt_avl_stack_pop(stack);
                        if(!entry) {
                                assert(0);
                                return NULL;
                        }
                        iface->set_root(tree, pmt_avl_node_remove_entry(
                                &iface->node_iface,  
                                stack, 
                                entry));
                        return entry;
                } else if(!pmt_avl_stack_push(stack, search)) {
                        assert(0);
                        return NULL;
                }
                search = iface->node_iface.get_left(search);
        }

        assert(0);
        return NULL;
}

void *pmt_avl_remove_max(
        pmt_avl_iface_t *iface,  
        void *tree,
        pmt_avl_stack_t *stack)
{
        assert(tree && pmt_avl_iface_validate(iface));

        void *search = iface->get_root(tree);

        if(!search) {
                return NULL;
        }

        pmt_avl_stack_reset(stack);

        if(!pmt_avl_stack_push(stack, search)) {
                assert(0);
                return NULL;
        }

        search = iface->node_iface.get_right(search);
        
        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {
                if(!search) {
                        void *entry = pmt_avl_stack_pop(stack);
                        if(!entry) {
                                assert(0);
                                return NULL;
                        }
                        iface->set_root(tree, pmt_avl_node_remove_entry(
                                &iface->node_iface, 
                                stack, 
                                entry));
                        return entry;
                } else if(!pmt_avl_stack_push(stack, search)) {
                        assert(0);
                        return NULL;
                }
                search = iface->node_iface.get_right(search);
        }

        assert(0);
        return NULL;
}

void *pmt_avl_min(pmt_avl_iface_t *iface,  void *tree)
{
        assert(tree && pmt_avl_iface_validate(iface));

        void *node = iface->get_root(tree);

        if(!node) {
                return NULL;
        }

        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {
                void *left = iface->node_iface.get_left(node);
                if(!left) {
                        return node;
                }
                node = left;
        }

        assert(0);
        return NULL;
}

void *pmt_avl_max(pmt_avl_iface_t *iface,  void *tree)
{
        assert(tree && pmt_avl_iface_validate(iface));

        void *node = iface->get_root(tree);

        if(!node) {
                return NULL;
        }

        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {
                void *right = iface->node_iface.get_right(node);
                if(!right) {
                        return node;
                }
                node = right;
        }

        assert(0);
        return NULL;
}

pmt_avl_stack_t *pmt_avl_entries(
        pmt_avl_iface_t *iface, 
        void *tree, 
        pmt_avl_stack_t *stack)
{
        assert(tree && pmt_avl_iface_validate(iface));

        void *node = iface->get_root(tree);

        pmt_avl_stack_reset(stack);

        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {
                if(!node) {
                        return stack;
                } 
                if(!pmt_avl_stack_push(stack, node)) {
                        assert(0);
                        return NULL;
                }
                node = iface->node_iface.get_left(node);
        }

        assert(0);
        return NULL;
}


pmt_avl_stack_t *pmt_avl_reversed(
        pmt_avl_iface_t *iface, 
        void *tree, 
        pmt_avl_stack_t *stack)
{
        assert(tree && pmt_avl_iface_validate(iface));

        void *node = iface->get_root(tree);

        pmt_avl_stack_reset(stack);

        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {
                if(!node) {
                        return stack;
                } 
                if(!pmt_avl_stack_push(stack, node)) {
                        assert(0);
                        return NULL;
                }
                node = iface->node_iface.get_right(node);
        }

        assert(0);
        return NULL;
}

bool pmt_avl_next(
        pmt_avl_iface_t *iface, 
        pmt_avl_stack_t *stack, 
        void **node)
{
        assert(pmt_avl_iface_validate(iface));

        void *next = pmt_avl_stack_pop(stack);
        
        if(!next) {
                return false;
        } 

        if(node) {
                *node = next;
        }

        void *scan = iface->node_iface.get_right(next);
        
        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {
                if(!scan) {
                        return true;
                }
                if(!pmt_avl_stack_push(stack, scan)) {
                        assert(0);
                        return false;
                }
                scan = iface->node_iface.get_left(scan);
        }

        assert(0);
        return false;
}

bool pmt_avl_prior(
        pmt_avl_iface_t *iface, 
        pmt_avl_stack_t *stack, 
        void **node)
{
        assert(pmt_avl_iface_validate(iface));

        void *next = pmt_avl_stack_pop(stack);
        
        if(!next) {
                return false;
        } 

        if(node) {
                *node = next;
        }

        void *scan = iface->node_iface.get_left(next);
        
        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {
                if(!scan) {
                        return true;
                }
                if(!pmt_avl_stack_push(stack, scan)) {
                        assert(0);
                        return false;
                }
                scan = iface->node_iface.get_right(scan);
        }

        assert(0);
        return false;
}

pmt_avl_stack_t *pmt_avl_upper(
        pmt_avl_iface_t *iface, 
        void *tree,
        pmt_avl_stack_t *stack,
        void *key)
{
        assert(pmt_avl_iface_validate(iface));

        pmt_avl_less_than_t less_than = iface->get_less_than(tree);

        void *search = iface->get_root(tree);

        pmt_avl_stack_reset(stack);

        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {

                if(!search) {
                        return stack;
                }

                void *search_key = iface->node_iface.get_key(search);

                if(less_than(key, search_key)) {
                        if(!pmt_avl_stack_push(stack, search)) {
                                assert(0);
                                return NULL;
                        }
                        search = iface->node_iface.get_left(search);
                } else if(less_than(search_key, key)) {
                        search = iface->node_iface.get_right(search);
                } else {
                        if(!pmt_avl_stack_push(stack, search)) {
                                assert(0);
                                return NULL;
                        }
                        return stack;
                }
        }

        assert(0);
        return NULL;
}

pmt_avl_stack_t *pmt_avl_lower(
        pmt_avl_iface_t *iface, 
        void *tree,
        pmt_avl_stack_t *stack,
        void *key)
{
        assert(pmt_avl_iface_validate(iface));

        pmt_avl_less_than_t less_than = iface->get_less_than(tree);

        void *search = iface->get_root(tree);

        pmt_avl_stack_reset(stack);

        for(int I = 0; I < PMT_AVL_STACK_MAX; ++I) {

                if(!search) {
                        return stack;
                }

                void *search_key = iface->node_iface.get_key(search);

                if(less_than(key, search_key)) {
                        search = iface->node_iface.get_left(search);
                } else if(less_than(search_key, key)) {
                        if(!pmt_avl_stack_push(stack, search)) {
                                assert(0);
                                return NULL;
                        }
                        search = iface->node_iface.get_right(search);
                } else {
                        if(!pmt_avl_stack_push(stack, search)) {
                                assert(0);
                                return NULL;
                        }
                        return stack;
                }
        }

        assert(0);
        return NULL;
}