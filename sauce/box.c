#include "box.h"
#include "vec.h"

/* piscina */
struct box box_pool_buf[BOX_POOL_BUF_LEN];
struct pool box_pool;

struct box* box_new(const float* pos, const float* size)
{
	struct box* b;
	int i, j;

	b = box_alloc();
	vec3_copy(pos, b->pos);
	vec3_copy(size, b->size);

	for (i = 0; i < 8; ++i)
		for (j = 0; j < 3; ++j)
			b->verts[i][j] = ((i & 1<<j) ? .5 : -.5) * size[j];

	return b;
}
