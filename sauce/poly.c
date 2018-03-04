#include <assert.h>
#include <stddef.h>
#include "poly.h"
#include "vec.h"
#include "poly_pool.h"

/*
 * cmp
 *
 */
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

void vert_cmp(struct vert_cmp* vc, struct vert* v, const float* p)
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

void edge_cmp(struct edge_cmp* ec, struct vert* v, const float* p)
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

/*
 * dup
 *
 */
struct vert* vert_dup(struct vert* v)
{
	struct vert* w;
	w = vert_alloc();
	(*w) = (*v);
	return w;
}

struct poly* poly_dup(struct poly* poly)
{
	struct poly* roly;
	roly = poly_alloc();
	(*roly) = (*poly);
	return roly;
}

/*
 * lerp
 *
 */
struct vert* vert_lerp(float w, struct vert* a, struct vert* b)
{
	struct vert* c;
	c = vert_dup(b);

	vec3_lerp(w, a->pos, c->pos);
	vec3_lerp(w, a->normal, c->normal);
	vec3_lerp(w, a->texcoords, c->texcoords);
	return c;
}

/*
 * split_winding
 *
 * divide lado/polígono por plano e retorna partes ordenadas pela ordem de
 * dobramento do vértices
 *
 */
void edge_split_winding(struct edge_cmp* ec, struct vert** a, struct vert** b)
{
	float w;

	/* BUGADO */
	assert (ec->side & SPLIT);

	/* divide a parada */
	(*a) = ec->vc[0].v;

	/* origem coincide com plano */
	if (ec->vc[0].side & ON_PLANE)
	{
		(*b) = vert_dup((*a));
		(*a)->next = NULL;
	}

	/* destino coincide com o plano */
	else if (ec->vc[1].side & ON_PLANE)
	{
		(*b) = vert_dup((*a)->next);
		(*a)->next->next = NULL;
	}

	/* ta cada 1 de 1 lado */
	else if ((ec->side & SPLIT) == SPLIT)
	{
		w = (ec->vc[1].h - ec->vc[1].p[3]) / (ec->vc[1].h - ec->vc[0].h);
		(*b) = vert_lerp(w, (*a), (*a)->next);
		(*b)->next = ec->vc[1].v;
		(*a)->next = vert_dup((*b));
		(*a)->next->next = NULL;
	}

	/* tudo no mesmo lado */
	else
	{
		(*b) = NULL;
	}
}

static inline int vert_count(struct vert* v)
{
	register int n;
	register struct vert* x;

	assert (v);

	for (n = 1, x = v->next; x && x != v; ++n, x = x->next);
	return n;
}

void poly_split_winding(struct poly_cmp* pc, struct poly** a, struct poly** b)
{
	struct vert* v[2][2];

	/* BUGADO */
	assert ((pc->side & SPLIT) == SPLIT);

	/* divide os lados */
	edge_split_winding(&pc->ec[0], v[0]+0, v[0]+1);
	edge_split_winding(&pc->ec[1], v[1]+0, v[1]+1);

	/* conecta os lados partidos */
	if (pc->ec[0].vc[0].side & ON_PLANE)
		v[0][0]->next = v[1][1];
	else
		v[0][0]->next->next = v[1][1];

	if (pc->ec[1].vc[0].side & ON_PLANE)
		v[1][0]->next = v[0][1];
	else
		v[1][0]->next->next = v[0][1];

	(*a) = pc->poly;
	(*b) = poly_dup(*a);
	(*b)->verts = v[0][1];
	(*a)->n_verts = vert_count((*a)->verts);
	(*b)->n_verts = vert_count((*b)->verts);
}

/*
 * split
 *
 * divide lado/polígono por plano e retorna partes ordenadas por trás e frente
 * do polígono
 *
 */

/* FIXME: void** */
static inline void sort_winding(enum side sa, enum side sb, void* a, void* b, void** back, void** front)
{
	if (sa & BACK || sb & FRONT)
	{
		(*back) = a;
		(*front) = b;
	}

	else
	{
		(*back) = b;
		(*front) = a;
	}
}

void edge_split(struct edge_cmp* ec, struct vert** back, struct vert** front)
{
	struct vert* a, * b;
	edge_split_winding(ec, &a, &b);
	/* FIXME: void** */
	sort_winding(ec->vc[0].side, ec->vc[1].side, a, b, (void**) back, (void**) front);
}

void poly_split(struct poly_cmp* pc, struct poly** back, struct poly** front)
{
	struct poly* a, * b;
	poly_split_winding(pc, &a, &b);
	/* FIXME: void** */
	sort_winding(pc->ec[0].vc[0].side, pc->ec[0].vc[1].side, a, b, (void**) back, (void**) front);
}
