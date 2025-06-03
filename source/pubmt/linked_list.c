
#include "pubmt/linked_list.h"
#include <assert.h>

static inline bool pmt_ll_node_i_tst(pmt_ll_node_iface *i)
{
        return i && i->get_next && i->set_next;
}

void *pmt_ll_node_last(pmt_ll_node_iface *iface, void *list)
{
        #ifndef NDEBUG
                if(!pmt_ll_node_i_tst(iface)) {
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
                if(!pmt_ll_node_i_tst(iface)) {
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
                if(!pmt_ll_node_i_tst(iface)) {
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
                if(!fst || !pmt_ll_node_i_tst(iface)) {
                        assert(0);
                        return NULL;
                }
        #endif
        iface->set_next(fst, list);
        return fst;
}

void *pmt_ll_node_push_back(pmt_ll_node_iface *iface, void *list, void *last)
{
        #ifndef NDEBUG 
                if(!last || !pmt_ll_node_i_tst(iface)) {
                        assert(0);
                        return NULL;
                }
        #endif
        iface->set_next(last, NULL);
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
                if(!node || !succ || !pmt_ll_node_i_tst(iface)) {
                        assert(0);
                        return NULL;
                }
        #endif 
        void *tmp = iface->get_next(node);
        iface->set_next(node, succ);
        iface->set_next(succ, tmp);
        return node;
}

void *pmt_ll_node_remove_after(pmt_ll_node_iface *iface, void *node)
{
        #ifndef NDEBUG
                if(!node || !pmt_ll_node_i_tst(iface)) {
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
                if(!pmt_ll_node_i_tst(iface)) {
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

void *pmt_ll_node_remove_last(pmt_ll_node_iface *iface, void **ref)
{
        #ifndef NDEBUG 
                if(!pmt_ll_node_i_tst(iface)) {
                        assert(0);
                        return NULL;
                }
        #endif
        void *node = *ref;
        if(!node) {
                return NULL;
        }
        void *pred = NULL;
        for(void *next; (next = iface->get_next(node));) {
                pred = node;
                node = next;
        }
        if(!pred) {
                *ref = NULL;
        } else {
                iface->set_next(pred, NULL);
        }
        return node;
}

void *pmt_ll_node_remove(pmt_ll_node_iface *iface, void **ref, void *node)
{
        #ifndef NDEBUG
                if(!node || !pmt_ll_node_i_tst(iface)) {
                        assert(0);
                        return NULL;
                }
        #endif
        void *list = *ref;
        if(!list) {
                return NULL;
        }
        if(node == list) {
                *ref = iface->get_next(node);
                iface->set_next(node, NULL);
                return node;
        }
        for(void *tmp; (tmp = iface->get_next(list)); ) {
                if(tmp == node) {
                        iface->set_next(list, iface->get_next(node));
                        iface->set_next(node, NULL);
                        return node;
                }
                list = tmp;
        }
        return NULL;
}

void *pmt_ll_node_remove_when(
        pmt_ll_node_iface *iface, 
        void **ref, 
        bool (*predicate)(void *node, void *state),
        void *state)
{
        #ifndef NDEBUG
                if(!predicate || !pmt_ll_node_i_tst(iface)) {
                        assert(0);
                        return NULL;
                } 
        #endif
        void *node = *ref;
        if(!node) {
                return NULL;
        }
        if(predicate(node, state)) {
                *ref = iface->get_next(node);
                iface->set_next(node, NULL);
                return node;
        }
        for(void *tmp; (tmp = iface->get_next(node)); ) {
                if(predicate(tmp, state)) {
                        iface->set_next(node, iface->get_next(tmp));
                        iface->set_next(tmp, NULL);
                        return tmp;
                }
                node = tmp;
        }
        return NULL;
}

size_t pmt_ll_node_filter(
        pmt_ll_node_iface *iface, 
        void **ref, 
        bool (*predicate)(void *node, void *state),
        void *state)
{
        #ifndef NDEBUG
                if(!predicate || !pmt_ll_node_i_tst(iface)) {
                        assert(0);
                        return 0;
                } 
        #endif
        void *node = *ref;
        void *first, *prev = NULL;
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
        *ref = first;
        return count;
}

void *pmt_ll_node_find(
        pmt_ll_node_iface *iface, 
        void *node,
        bool (*predicate)(void *node, void *state),
        void *state)
{
        #ifndef NDEBUG
                if(!predicate || !pmt_ll_node_i_tst(iface)) {
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
                if(!callback || !pmt_ll_node_i_tst(iface)) {
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
                if(!pmt_ll_node_i_tst(iface)) {
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
