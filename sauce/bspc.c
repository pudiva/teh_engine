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

#ifdef DEBUG
#include <stdio.h>
#define p(c) fputc(s, stderr)
#else
#define p(c) ((void)0);
#endif

#include "bspc.h"
#include "vec.h"

struct node node_pool[BSPC_NODE_POOL_LEN];
int node_pool_c = 0;

struct node* node_alloc()
{
	assert (node_pool_c < BSPC_NODE_POOL_LEN);
	memset(&node_pool[node_pool_c], 0, sizeof (struct node));
	return &node_pool[node_pool_c++];
}

void node_free(struct node* node)
{
	/* TODO */
}

/*
 * compiler!11!!1
 *
 */
struct node* bspc(struct tri* list)
{
	struct tri* splitter, * best, * cur, * save;
	struct tri* back, ** pback, * front, ** pfront;
	int score, score_best;
	bool is_convex;
	struct node* node;

	p('+');
	node = node_alloc();

	/* uma folha sólida */
	if (!list)
	{
		p('S');
		node->is_leaf = true;
		node->is_solid = true;
		return node;
	}

	/* encontra melhor divisor e determina se a lista é convexa */
	is_convex = true;
	best = NULL;
	score_best = INT_MAX;
	for (splitter = list; splitter; splitter = splitter->next)
	{
		p('x');

		if (splitter->used)
			continue;

		score = 0;
		for (cur = list; cur; cur = cur->next)
		{
			p('t');
			assert (cur->next != cur);

			tri_split_prepare(cur, splitter->p);
			score += tri_split_score;

			if (0 < tri_split_count)
				is_convex = false;
		}

		if (score < score_best)
		{
			best = splitter;
			score_best = score;
		}
	}

	/* folha líquida */
	if (is_convex)
	{
		p('L');
		node->is_leaf = true;
		node->is_solid = false;
		node->tris = list;
		return node;
	}

	p('N');

	back = NULL;
	front = NULL;
	pback = &back;
	pfront = &front;

	vec4_copy(best->p, node->plane);

	/* divide todos os triangulos com o melhor divisor encontrado */
	for (cur = list; cur; cur = save)
	{
		assert (cur != cur->next);
		save = cur->next;
		cur->next = NULL;

		tri_split_prepare(cur, best->p);
		tri_split();
		p('s');

		/* lista de tras */
		if (tri_split_parts[0])
		{
			(*pback) = tri_split_parts[0];

			while (*pback)
				pback = &(*pback)->next;
		}

		/* lista do plano -> lista da frente */
		if (tri_split_parts[1])
		{
			(*pfront) = tri_split_parts[1];

			while (*pfront)
			{
				/* marca tudo nesse plano como usado! */
				(*pfront)->used = true;
				pfront = &(*pfront)->next;
			}
		}

		/* lista da frente */
		if (tri_split_parts[2])
		{
			(*pfront) = tri_split_parts[2];

			while (*pfront)
				pfront = &(*pfront)->next;
		}
	}

	/* inception */
	node->back = bspc(back);
	node->front = bspc(front);
	return node;
}

/*
 * compila teh
 *
 */
struct node* bspc_teh(struct teh* model)
{
	struct tri* list;
	struct node* node;

	list = tri_from_teh(model);
	assert (list);

	node = bspc(list);
	assert (node);

	return node;
}

/*
 * converte para teh
 *
 */
static inline int convert_node(
		int i,
		int k,
		struct beh_node* d,
		float (*tc)[3][2],
		float (*v)[3][3],
		struct node* s
		)
{
	int j;
	struct tri* t;

	d[i].is_leaf = s[i].is_leaf;
	d[i].is_solid = s[i].is_solid;
	vec4_copy(s[i].plane, d[i].plane);
	d[i].back = s[i].back ? d + (s[i].back - s) : NULL;
	d[i].front = s[i].front ? d + (s[i].front - s) : NULL;

	/* folha líquida */
	if (s[i].is_leaf && !s[i].is_solid)
	{
		d[i].off = k;
		d[i].size = 0;

		for (t = s[i].tris; t; t = t->next)
		{
			for (j = 0; j < 3; ++j)
			{
				vec2_copy(t->tc[j], tc[k][j]);
				vec3_copy(t->v[j], v[k][j]);
			}
			++k;
			++d[i].size;
		}
	}

	return k;
}

struct beh* node_pool_to_beh()
{
	int i, k;
	struct beh* bsp;

	bsp = calloc(1, sizeof (struct beh));

	bsp->model.n_tris = tri_pool_c;
	bsp->model.n_frames = 1;
	bsp->model.tris = calloc(bsp->model.n_tris, sizeof (float[3][3]));
	bsp->model.texcoords = calloc(bsp->model.n_tris, sizeof (float[3][2]));
	bsp->n_nodes = node_pool_c;
	bsp->nodes = calloc(node_pool_c, sizeof (struct beh_node));

	k = 0;
	for (i = 0; i < node_pool_c; ++i)
		k = convert_node(i, k, bsp->nodes, bsp->model.texcoords, bsp->model.tris, node_pool);

	assert (k == tri_pool_c);

	return bsp;
}

/*
 * compila e converte
 *
 */
struct beh* behc(struct teh* model)
{
	struct node* root;
	struct beh* bsp;

	root = bspc_teh(model);
	assert (root);

	bsp = node_pool_to_beh();
	assert (bsp);

	return bsp;
}
