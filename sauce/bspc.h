#ifndef HAS_BSPC_H
#define HAS_BSPC_H

#include "beh.h"
#include "poly.h"

/*
 * piscina de nodos jajajajaj
 *
 */
#define BSPC_NODE_POOL_LEN 66666

extern struct beh_node node_pool[BSPC_NODE_POOL_LEN];
extern int node_pool_c;

struct beh_node* node_alloc();

struct beh_node* behc_node(struct poly* list);
struct beh* node_pool_to_beh();
struct beh* behc(struct teh* model);

#endif
