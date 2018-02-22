#ifndef HAS_TRI_H
#define HAS_TRI_H

#include <stdbool.h>
#include "teh.h"

/* NOTE: nomes curtos pq se nao fica tenso */
struct tri
{
	bool used;
	float v[3][3];
	float tc[3][2];
	float p[4];
	struct tri* next;
};

/*
 * piscina de triangulinhos jajajaja
 *
 */
#define TRI_POOL_LEN 66666

extern struct tri tri_pool[TRI_POOL_LEN];
extern int tri_pool_c;

struct tri* tri_alloc();

struct tri* tri_from_teh(struct teh* model);
struct teh* tri_to_teh(struct tri* list);

/*
 * tri split
 *
 */
extern int tri_split_count, tri_split_n_parts[3], tri_split_score, tri_split_facing;
extern struct tri* tri_split_parts[3];

void tri_split_prepare(struct tri* src, float* plane);
void tri_split();

#endif
