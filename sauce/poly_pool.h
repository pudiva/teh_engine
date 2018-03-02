#ifndef HAS_POLY_POOL_H
#define HAS_POLY_POOL_H

#include "poly.h"

#define POLY_POOL_SIZE 66666
#define VERT_POOL_SIZE 66666

struct poly* poly_alloc();
void poly_free(struct poly* poly);

struct vert* vert_alloc();
void vert_free(struct vert* vert);

void poly_pool_init();

#endif
