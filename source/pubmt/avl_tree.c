
#include "pubmt/avl_tree.h"

static inline size_t pmt_avl_node_height(pmt_avl_iface_t *iface, void *node)
{
        return node ? iface->get_height(node) : 0;
}

static inline void pmt_avl_node_update_height(pmt_avl_iface_t *iface, void *node)
{
        const size_t 
                h_l = pmt_avl_node_height(iface, iface->get_left(node)),
                h_r = pmt_avl_node_height(iface, iface->get_right(node));

        iface->set_height(node, 1 + (h_l < h_r ? h_r : h_l));
}

static void *pmt_avl_rotate_left(pmt_avl_iface_t *iface, void *node)
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

static void *pmt_avl_rotate_right(pmt_avl_iface_t *iface, void *node)
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