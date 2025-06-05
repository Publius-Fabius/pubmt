
#include "pubmt/linked_list.h"
#include <assert.h>

static inline bool pmt_ll_node_iface_tst(pmt_ll_node_iface *i)
{
        return i && i->get_next && i->set_next;
}

void *pmt_ll_node_last(pmt_ll_node_iface *iface, void *list)
{
        #ifndef NDEBUG
                if(!pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
        #endif
        void *node = NULL;
        for(; list; list = iface->get_next(list)) {
                node = list;
        }
        return node;
}

size_t pmt_ll_node_count(pmt_ll_node_iface *iface, void *list)
{
        #ifndef NDEBUG
                if(!pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return 0;
                } 
        #endif
        size_t count = 0;
        for(; list; list = iface->get_next(list)) {
                ++count;
        }
        return count;
}

void *pmt_ll_node_at(pmt_ll_node_iface *iface, void *list, const size_t index)
{
        #ifndef NDEBUG
                if(!pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return 0;
                } 
        #endif
        for(size_t n = 0; n < index; ++n) {
                if(!list) {
                        return NULL;
                } else {
                        list = iface->get_next(list);
                }
        }
        return list;
}

void *pmt_ll_node_push_front(pmt_ll_node_iface *iface, void *list, void *fst)
{
        #ifndef NDEBUG 
                if(!fst || !pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                }
        #endif
        assert(iface->get_next(fst) == NULL);
        iface->set_next(fst, list);
        return fst;
}

void *pmt_ll_node_push_back(pmt_ll_node_iface *iface, void *list, void *last)
{
        #ifndef NDEBUG 
                if(!last || !pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                }
        #endif
        if(!list) {
                return last;
        }
        void *node = list;
        for(void *next; (next = iface->get_next(node)); ) {
                node = next;
        }
        iface->set_next(node, last);
        return list;
}

void *pmt_ll_node_insert_after(
        pmt_ll_node_iface *iface, 
        void *node, 
        void *succ)
{
        #ifndef NDEBUG 
                if(!node || !succ || !pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                }
        #endif 
        assert(iface->get_next(succ) == NULL);
        void *tmp = iface->get_next(node);
        iface->set_next(node, succ);
        iface->set_next(succ, tmp);
        return node;
}

void *pmt_ll_node_remove_after(pmt_ll_node_iface *iface, void *node)
{
        #ifndef NDEBUG
                if(!node || !pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                }
        #endif
        void *succ = iface->get_next(node);
        if(!succ) {
                return NULL;
        } else {
                iface->set_next(node, iface->get_next(succ));
                iface->set_next(succ, NULL);
                return succ;
        }
}

void *pmt_ll_node_remove_first(pmt_ll_node_iface *iface, void **ref)
{
        #ifndef NDEBUG 
                if(!pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                }
        #endif
        void *node = *ref;
        if(!node) {
                return NULL;
        }
        *ref = iface->get_next(node);
        iface->set_next(node, NULL);
        return node; 
}

void *pmt_ll_node_remove_last_args(
        pmt_ll_node_iface *iface, 
        void **list_ref,
        void **pred_ref)
{
        #ifndef NDEBUG 
                if(!pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                }
        #endif
        void *node = *list_ref;
        if(!node) {
                *pred_ref = NULL;
                return NULL;
        }
        void *pred = NULL;
        for(void *next; (next = iface->get_next(node));) {
                pred = node;
                node = next;
        }
        if(!pred) {
                *list_ref = NULL;
                *pred_ref = NULL;
        } else {
                *pred_ref = pred;
                iface->set_next(pred, NULL);
        }
        return node;
}

void *pmt_ll_node_remove_last(pmt_ll_node_iface *iface, void **ref)
{
        void *pred_ref;
        return pmt_ll_node_remove_last_args(iface, ref, &pred_ref);
}

void *pmt_ll_node_remove_when_args(
        pmt_ll_node_iface *iface, 
        void **list_ref, 
        bool (*predicate)(void *node, void *state),
        void *state,
        void **pred_ref)
{
        #ifndef NDEBUG
                if(!predicate || !pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
        #endif
        void *node = *list_ref;
        if(!node) {
                *pred_ref = NULL;
                return NULL;
        }
        if(predicate(node, state)) {
                *list_ref = iface->get_next(node);
                iface->set_next(node, NULL);
                *pred_ref = NULL;
                return node;
        }
        for(void *next; (next = iface->get_next(node)); ) {
                if(predicate(next, state)) {
                        iface->set_next(node, iface->get_next(next));
                        iface->set_next(next, NULL);
                        *pred_ref = node;
                        return next;
                }
                node = next;
        }
        *pred_ref = NULL;
        return NULL;
}

void *pmt_ll_node_remove_when(
        pmt_ll_node_iface *iface, 
        void **ref, 
        bool (*predicate)(void *node, void *state),
        void *state)
{
        void *pred_ref = NULL;
        return pmt_ll_node_remove_when_args(
                iface, ref, predicate, state, &pred_ref);
}

size_t pmt_ll_node_filter_args(
        pmt_ll_node_iface *iface, 
        void **first_ref, 
        bool (*predicate)(void *node, void *state),
        void *state,
        void **last_ref)
{
        #ifndef NDEBUG
        if(!predicate || !pmt_ll_node_iface_tst(iface)) {
                assert(0);
                return 0;
        } 
        #endif
        void *node = *first_ref;
        void *first = NULL;
        void *prev = NULL;
        size_t count = 0;
        while(node) {
                void *next = iface->get_next(node);
                if(!predicate(node, state)) {
                        ++count;
                        iface->set_next(node, NULL);
                        if(prev) {
                                iface->set_next(prev, next);
                        }
                } else {
                        if(!first) {
                                first = node;
                        } 
                        prev = node;
                }
                node = next;   
        }
        *first_ref = first;
        *last_ref = prev;
        return count;
}

size_t pmt_ll_node_filter(
        pmt_ll_node_iface *iface, 
        void **list, 
        bool (*predicate)(void *node, void *state),
        void *state)
{
        void *pred_ref = NULL;
        return pmt_ll_node_filter_args(
                iface, list, predicate, state, &pred_ref);
}

void *pmt_ll_node_find(
        pmt_ll_node_iface *iface, 
        void *node,
        bool (*predicate)(void *node, void *state),
        void *state)
{
        #ifndef NDEBUG
                if(!predicate || !pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
        #endif 
        for(; node; node = iface->get_next(node)) {
                if(predicate(node, state)) {
                        return node;
                }
        }
        return NULL;
}

bool pmt_ll_node_foreach(
        pmt_ll_node_iface *iface, 
        void *node, 
        bool (*callback)(void *node, void *state),
        void *state)
{
        #ifndef NDEBUG
                if(!callback || !pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return false;
                }
        #endif
        for(; node; node = iface->get_next(node)) {
                if(!callback(node, state)) {
                        return false;
                }
        }
        return true;
}

void *pmt_ll_node_reverse(pmt_ll_node_iface *iface, void *list)
{
        #ifndef NDEBUG
                if(!pmt_ll_node_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
        #endif
        void *head = NULL;
        while(list) {
                void *next = iface->get_next(list);
                iface->set_next(list, head);
                head = list;
                list = next;
        }
        return head;
}

static inline bool pmt_ll_iface_tst(pmt_ll_iface *i)
{
        return  i && 
                i->get_first && 
                i->set_first && 
                i->get_last && 
                i->set_last;
}

void *pmt_ll_push_front(pmt_ll_iface *iface, void *list, void *first)
{
        #ifndef NDEBUG
                if(!list || !first || !pmt_ll_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
                if(!pmt_ll_node_iface_tst(&iface->node_iface)) {
                        assert(0);
                        return NULL;
                }
        #endif

        pmt_ll_node_iface *n_iface = &iface->node_iface;

        assert(n_iface->get_next(first) == NULL);
        
        void *node = iface->get_first(list);

        if(!node) {
                assert(iface->get_last(list) == NULL);
                iface->set_last(list, first);
        } else {
                n_iface->set_next(first, node);
        }

        iface->set_first(list, first);

        return list;
}

void *pmt_ll_push_back(pmt_ll_iface *iface, void *list, void *last)
{
        #ifndef NDEBUG
                if(!list || !last || !pmt_ll_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
                if(!pmt_ll_node_iface_tst(&iface->node_iface)) {
                        assert(0);
                        return NULL;
                }
        #endif

        pmt_ll_node_iface *n_iface = &iface->node_iface;

        void *node = iface->get_last(list);

        if(node == NULL) {
                assert(iface->get_first(list) == NULL);
                iface->set_first(list, last);
        } else {
                assert(n_iface->get_next(node) == NULL);
                n_iface->set_next(node, last);
        }

        iface->set_last(list, pmt_ll_node_last(n_iface, last));

        return list;
}

void *pmt_ll_insert_after(
        pmt_ll_iface *iface, 
        void *list, 
        void *node,
        void *succ)
{
        #ifndef NDEBUG
                if(!list || !pmt_ll_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
        #endif

        void *last = iface->get_last(list);

        assert(last);

        (void)pmt_ll_node_insert_after(&iface->node_iface, node, succ);

        if(node == last) {

                /* succ must be unlinked by node_insert_after's contract. */

                iface->set_last(list, succ);
        }

        return list;
}

void *pmt_ll_remove_after(pmt_ll_iface *iface, void *list, void *node)
{
        #ifndef NDEBUG
                if(!list || !pmt_ll_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
        #endif 

        void *last = iface->get_last(list);

        assert(last);

        void *dropped = pmt_ll_node_remove_after(&iface->node_iface, node);

        if(!dropped) {
                return NULL;
        } 
        if(dropped == last) {
                iface->set_last(list, node);
        }

        return dropped;
}

void *pmt_ll_remove_first(pmt_ll_iface *iface, void *list)
{
        #ifndef NDEBUG
                if(!list || !pmt_ll_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
                if(!pmt_ll_node_iface_tst(&iface->node_iface)) {
                        assert(0);
                        return NULL;
                }
        #endif

        pmt_ll_node_iface *n_iface = &iface->node_iface;
        void *first = iface->get_first(list);
        void *last = iface->get_last(list);

        if(!first) {
                assert(last == NULL);
                return NULL;
        } else if(first == last) {
                assert(n_iface->get_next(last) == NULL);
                iface->set_first(list, NULL);
                iface->set_last(list, NULL);
        } else {
                iface->set_first(list, n_iface->get_next(first));
                n_iface->set_next(first, NULL);
        }

        return first;
}

void *pmt_ll_remove_last(pmt_ll_iface *iface, void *list)
{
        #ifndef NDEBUG
                if(!list || !pmt_ll_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
        #endif

        void *first = iface->get_first(list);
        void *last = iface->get_last(list);

        if(!first) {
                assert(last == NULL);
                return NULL;
        } else if(first == last) {
                iface->set_first(list, NULL);
                iface->set_last(list, NULL);
                return last;
        } 

        void *first_ref = first, *pred_ref = NULL;

        void *drop = pmt_ll_node_remove_last_args(
                &iface->node_iface, &first_ref, &pred_ref);

        assert(first_ref == first && drop && drop == last && pred_ref);

        iface->set_last(list, pred_ref);
        
        return last;
}

void *pmt_ll_remove_when(
        pmt_ll_iface *iface, 
        void *list, 
        bool (*predicate)(void *node, void *state),
        void *state)
{
        #ifndef NDEBUG
                if(!list || !pmt_ll_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
        #endif

        void *first = iface->get_first(list);
        void *last = iface->get_last(list);

        void *first_ref = first, *pred_ref = NULL;

        void *drop = pmt_ll_node_remove_when_args(
                &iface->node_iface, &first_ref, predicate, state, &pred_ref);

        if(!drop) {
                return NULL;
        } else if(first == last) {
                assert(drop == last);
                iface->set_first(list, NULL);
                iface->set_last(list, NULL);
                return last;
        } else if(drop == first) {
                iface->set_first(list, first_ref);
        } else if(drop == last) {
                iface->set_last(list, pred_ref);
        } 

        return drop;
}

size_t pmt_ll_filter(
        pmt_ll_iface *iface, 
        void *list, 
        bool (*predicate)(void *node, void *state),
        void *state)
{
        #ifndef NDEBUG
                if(!list || !pmt_ll_iface_tst(iface)) {
                        assert(0);
                        return 0;
                } 
        #endif

        void *first = iface->get_first(list), *last = NULL;

        const size_t removed = pmt_ll_node_filter_args(
                &iface->node_iface, &first, predicate, state, &last);

        iface->set_first(list, first);
        iface->set_last(list, last);

        return removed;
}

void *pmt_ll_reverse(pmt_ll_iface *iface, void *list)
{
        #ifndef NDEBUG
                if(!list || !pmt_ll_iface_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
        #endif

        void *first = iface->get_first(list);
        void *last = iface->get_last(list);

        (void)pmt_ll_node_reverse(&iface->node_iface, first);

        iface->set_first(list, last);
        iface->set_last(list, first);

        return list;
}