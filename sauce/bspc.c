/*
 * referências:
 *
 * http://www.cs.utah.edu/~jsnider/SeniorProj/BSP1/default.html
 * http://www.cs.utah.edu/~jsnider/SeniorProj/BSP/default.htm
 * https://github.com/id-Software/Quake-III-Arena
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
	best->used = true;

	/* divide todos os triangulos com o melhor divisor encontrado */
	for (cur = list; cur; cur = save)
	{
		assert (cur != cur->next);
		save = cur->next;
		cur->next = NULL;

		tri_split_prepare(cur, best->p);
		tri_split();
		p('s');

		assert (!tri_split_parts[0] || tri_split_parts[0] != tri_split_parts[1]);
		assert (!tri_split_parts[1] || tri_split_parts[1] != tri_split_parts[2]);
		assert (!tri_split_parts[2] || tri_split_parts[2] != tri_split_parts[0]);

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
				pfront = &(*pfront)->next;
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
