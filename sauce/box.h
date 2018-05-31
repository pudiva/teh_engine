#ifndef HAS_BOX_H
#define HAS_BOX_H

#include "pool.h"

/* 1 caixa (aabb) */
struct box
{
	float pos[3];	/* posição do centro da caixa */
	float size[3];	/* dimensões */

	/* vértices da caixa.
	 *
	 * o primeiro índice é mágico e codifica um vetor de 3bits que
	 * representa os sinais dos componentes do vertice.
	 *
	 * por exemplo:
	 *
	 * o índice 4 é equivalente ao vetor (0, 0, 1), portanto:
	 *
	 * vert[4][0] < 0
	 * vert[4][1] < 0
	 *              0 < vert[4][2]
	 *
	 */
	float verts[8][3];
};

/* piscina */
#define BOX_POOL_BUF_LEN 666
extern struct box box_pool_buf[BOX_POOL_BUF_LEN];
extern struct pool box_pool;

#define box_pool_clear() pool_clear(&box_pool)
#define box_alloc() pool_alloc(&box_pool)
#define box_free(x) pool_free(&box_pool, (x))
#define box_pool_init() pool_init(&box_pool, BOX_POOL_BUF_LEN, sizeof (struct box), box_pool_buf)
#define box_pool_fini() pool_free(&box_pool)

struct box* box_new(const float* pos, const float* size);

#endif
