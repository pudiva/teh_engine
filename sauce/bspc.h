#ifndef HAS_BSPC_H
#define HAS_BSPC_H

#include "beh.h"
#include "poly.h"

/*
 * piscina de nodos jajajajaj
 *
 */
#define BEH_NODE_POOL_BUF_LEN 66666

extern struct beh_node beh_node_pool_buf[BEH_NODE_POOL_BUF_LEN];
extern struct pool beh_node_pool;

#define beh_node_pool_clear() pool_clear(&beh_node_pool)
#define beh_node_alloc() pool_alloc(&beh_node_pool)
#define beh_node_free(x) pool_free(&beh_node_pool, (x))
#define beh_node_pool_init() pool_init(&beh_node_pool, BEH_NODE_POOL_BUF_LEN, sizeof (struct beh_node), beh_node_pool_buf)
#define beh_node_pool_fini() pool_free(&beh_node_pool)

struct beh_node* behc_node(struct poly* list);
struct beh* node_pool_to_beh();
struct beh* behc(struct teh* model);

#endif
