/*
 * referências:
 *
 * http://www.cs.utah.edu/~jsnider/SeniorProj/BSP1/default.html
 * http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
 * https://github.com/id-Software/Quake-III-Arena
 *
 * TODO: fazer interface como máquina de estados.
 * TODO: função para reiniciar o estado.
 *
 */

#include <assert.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

//#define PRINTS

#ifdef PRINTS
#include <stdio.h>
#define p(c) fputc(c, stderr)
#else
#define p(c) ((void)0);
#endif

#include "poly.h"
#include "bspc.h"
#include "vec.h"
#include "beh.h"

/*
 * piscina de nodos
 *
 */
struct beh_node beh_node_pool_buf[BEH_NODE_POOL_BUF_LEN] = {0};
struct pool beh_node_pool;

/*
 * pontua divisor
 *
 */
static inline int split_score(struct poly* list, const float* p)
{
	struct poly_cmp pc;
	struct poly* cur;
	int back, front, split;

	back = 0;
	front = 0;
	split = 0;
	for (cur = list; cur; cur = cur->next)
	{
		//p('t');
		poly_cmp(&pc, cur, p);

		switch (pc.side & SPLIT)
		{
		case BACK:  ++back;  break;
		case FRONT: ++front; break;
		case SPLIT: ++split; break;
		}
	}

	return abs(back - front) * 8 + split * 6;
}

/*
 * encontra divisor com melhor (menor) pontuação
 *
 */
static inline struct poly* best_splitter(struct poly* list)
{
	struct poly* cur, * best;
	int score_best, score_cur;

	assert (list);

	best = NULL;
	score_best = INT_MAX;
	for (cur = list; cur; cur = cur->next)
	{
		assert (cur->next != cur);
		//p('x');

		if (cur->used)
			continue;

		score_cur = split_score(list, cur->p);

		if (score_cur < score_best)
		{
			best = cur;
			score_best = score_cur;
		}
	}

	return best;
}

/*
 * divide 1 polígono
 *
 */
static inline void split_one_poly(struct poly* poly, const float* p, struct poly** parts)
{
	struct poly_cmp pc;

	assert (poly->next != poly);
	assert (p[0] || p[1] || p[2]);

	poly_cmp(&pc, poly, p);

	parts[0] = NULL;
	parts[1] = NULL;

	switch (pc.side)
	{
	/* polígono exatamente no plano */
	case ON_PLANE:
		poly->used = true;

		/* apontando pro mesmo lado */
		if (0 <= vec3_dot(poly->p, p))
			parts[1] = poly;

		/* apontando pra trás */
		else
			parts[0] = poly;

		break;

	/* polígono dividido */
	case SPLIT:
	case ON_PLANE | SPLIT:
		poly_split(&pc, parts+0, parts+1);
		p('|');
		break;

	/* polígono atrás */
	case BACK:
	case ON_PLANE | BACK:
		parts[0] = poly;
		p('<');
		break;

	/* só sobrou a frente... eu acho... */
	default:
		parts[1] = poly;
		p('>');
		break;
	}
}

/*
 * divide lista de poligonosss
 *
 */
static inline void split_polys(struct poly* poly, const float* p, struct poly** parts)
{
	struct poly* cur, * next, * new[2];

	parts[0] = NULL;
	parts[1] = NULL;
	for (cur = poly; cur; cur = next)
	{
		next = cur->next;
		assert (cur != next);

		split_one_poly(cur, p, new);

		if (new[0])
		{
			new[0]->next = parts[0];
			parts[0] = new[0];
		}

		if (new[1])
		{
			new[1]->next = parts[1];
			parts[1] = new[1];
		}
	}
}

/*
 * compiler!11!!1
 *
 */
struct beh_node* behc_node(struct poly* list)
{
	struct poly* parts[2], * best;
	struct beh_node* node;

	p('+');
	node = beh_node_alloc();

	/* uma folha sólida */
	if (!list || !(best = best_splitter(list)))
	{
		node->type = LEAF;
		node->polys = list;
		return node;
	}

	p('N');
	vec4_copy(best->p, node->plane);
	split_polys(list, best->p, parts);

	/* inception */
	node->kids[0] = behc_node(parts[0]);
	node->kids[1] = behc_node(parts[1]);

	if (node->kids[0]->type == LEAF)
	{
		node->kids[0]->type = SOLID_LEAF;
		p('S');
		node->kids[0]->polys = NULL; /* FIXME: leaking */
	}

	if (node->kids[1]->type == LEAF)
	{
		node->kids[1]->type = LIQUID_LEAF;
		p('L');
	}

	return node;
}

/*
 * poly_from_teh
 *
 */
struct poly* poly_from_teh(struct teh* model)
{
	int i, j;
	struct poly* list;
	struct poly** pp;
	struct vert** pv;
	float e1[3], e2[3];

	pp = &list;
	list = NULL;
	for (i = 0; i < model->n_tris; ++i)
	{
		(*pp) = poly_alloc();
		(**pp).n_verts = 3;
		(**pp).next = NULL;
		(**pp).used = false;

		/* copia os vertice */
		for (j = 0, pv = &(**pp).verts; j < 3; ++j, pv = &(**pv).next)
		{
			(*pv) = vert_alloc();
			vec3_copy(model->tris[i][j], (**pv).pos);
			vec2_copy(model->texcoords[i][j], (**pv).texcoords);
		}

		/* fecha o ciclo */
		(*pv) = (**pp).verts;

		vec3_copy((**pp).verts->next->pos, e1);
		vec3_axpy(-1, (**pp).verts->pos, e1);

		vec3_copy((**pp).verts->next->next->pos, e2);
		vec3_axpy(-1, (**pp).verts->pos, e2);

		vec3_cross(e1, e2, (**pp).p);
		vec3_normalize((**pp).p);
		(**pp).p[3] = vec3_dot((**pp).verts->pos, (**pp).p);

		assert ((**pp).next != (*pp));
		pp = &(**pp).next;
	}

	return list;
}

/*
 * compila teh
 *
 */
struct beh_node* bspc_teh(struct teh* model)
{
	struct poly* list;
	struct beh_node* node;

	list = poly_from_teh(model);
	assert (list);

	beh_node_pool_clear();

	node = behc_node(list);
	assert (node);

	return node;
}

/*
 * conta triângulos
 *
 */
static inline int count_tris(struct beh_node* node)
{
	struct poly* cur;
	int n;

	if (!node)
		return 0;

	n = 0;
	if (node->type == LIQUID_LEAF)
		for (cur = node->polys; cur; cur = cur->next)
			n += cur->n_verts-2;

	n += count_tris(node->kids[0]);
	n += count_tris(node->kids[1]);
	return n;
}

static inline void put_poly_tris(struct teh* teh, struct poly* poly)
{
	register int i;
	struct vert* v[3];

	v[0] = poly->verts;
	i = 0;
	for (v[1] = v[0]->next, v[2] = v[1]->next; v[2] != v[0]; v[1] = v[1]->next, v[2] = v[2]->next)
	{
		for (i = 0; i < 3; ++i)
		{
			vec3_copy(v[i]->pos, teh->tris[teh->n_tris][i]);
			vec2_copy(v[i]->texcoords, teh->texcoords[teh->n_tris][i]);
		}
		++teh->n_tris;
	}
}

static inline void put_node_tris(struct teh* teh, struct beh_node* node)
{
	struct poly* cur;

	if (!node)
		return;

	node->i[0] = teh->n_tris;

	if (node->type == LIQUID_LEAF)
		for (cur = node->polys; cur; cur = cur->next)
			put_poly_tris(teh, cur);

	node->i[1] = teh->n_tris - node->i[0];

	put_node_tris(teh, node->kids[0]);
	put_node_tris(teh, node->kids[1]);
}

struct beh* node_pool_to_beh()
{
	int n_tris;
	struct beh* bsp;

	bsp = calloc(1, sizeof (struct beh));

	n_tris = count_tris(beh_node_pool_buf+0);
	bsp->model.tris = calloc(n_tris, sizeof (float[3][3]));
	bsp->model.texcoords = calloc(n_tris, sizeof (float[3][2]));

	put_node_tris(&bsp->model, beh_node_pool_buf+0);
	assert (bsp->model.n_tris == n_tris);

	bsp->model.n_frames = 1;

	bsp->n_nodes = beh_node_pool.n_used;
	bsp->nodes = beh_node_pool_buf;
	return bsp;
}

/*
 * compila e converte
 *
 */
struct beh* behc(struct teh* model)
{
	struct beh_node* root;
	struct beh* bsp;

	beh_node_pool_init();

	root = bspc_teh(model);
	assert (root);

	bsp = node_pool_to_beh();
	assert (bsp);

	return bsp;
}
