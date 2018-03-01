#include <assert.h>
#include <stdio.h>
#include "poly.h"
#include "vec.h"

static inline enum side cmp2side(int c)
{
	switch (c)
	{
	case -1: return BACK;
	case  0: return ON_PLANE;
	case  1: return FRONT;
	}

	assert (false);
	return -1;
}

void vert_cmp(struct vert_cmp* vc, const struct vert* v, const float* p)
{
	vc->v = v;
	vc->p = p;
	vc->h = vec3_dot(v->pos, p);
	vc->c = vec_cmp(vc->h, p[3]);
	vc->side = cmp2side(vc->c);
}

void edge_cmp_next(struct edge_cmp* ec)
{
	ec->vc[0] = ec->vc[1];
	vert_cmp(ec->vc + 1, ec->vc[0].v->next, ec->vc[0].p);
	ec->side = ec->vc[0].side | ec->vc[1].side;
}

void edge_cmp(struct edge_cmp* ec, const struct vert* v, const float* p)
{
	vert_cmp(ec->vc + 1, v, p);
	edge_cmp_next(ec);
}

void poly_cmp(struct poly_cmp* pc, struct poly* poly, const float* p)
{
	int i;
	struct edge_cmp* ec;

	pc->poly = poly;
	ec = pc->ec;
	vert_cmp(ec->vc + 1, poly->verts, p);
	pc->side = 0;

	for (i = 0; i < poly->n_verts; ++i)
	{
		edge_cmp_next(ec);
		pc->side |= ec->side;

		if (ec->vc[0].side & ON_PLANE || (ec->side & SPLIT) == SPLIT)
		{
			++ec;

			if (1 < ec - pc->ec)
				break;

			ec[0].vc[1] = ec[-1].vc[1];
		}
	}
}
