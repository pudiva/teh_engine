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
	struct tri* splitter, * best, * cur, * next;
	struct tri* back, ** pback, * front, ** pfront;
	int score, score_best;
	int is_concave;
	struct node* node;

	node = node_alloc();

	/* uma folha sólida */
	if (!list)
	{
		node->is_leaf = true;
		node->is_solid = true;
		return node;
	}

	/* encontra melhor divisor e determina se a lista é convexa */
	is_concave = 0;
	for (splitter = list; splitter; splitter = splitter->next)
	{
		score = 0;
		for (cur = list; cur; cur = cur->next)
		{
			/* diviception */
			if (cur == splitter || cur->used)
				continue;

			tri_split_prepare(list, splitter->p);
			score += tri_split_score;
			is_concave += tri_split_count;
		}

		if (score < score_best)
		{
			best = splitter;
			score_best = score;
		}
	}

	/* folha líquida */
	if (!is_concave)
	{
		node->is_leaf = true;
		node->is_solid = false;
		node->tris = list;
		return node;
	}

	pback = &back;
	pfront = &front;

	/* divide todos os triangulos com o melhor divisor encontrado */
	for (cur = list, next = cur->next; cur; cur = next, next = cur->next)
	{
		/* diviception */
		if (cur == splitter || cur->used)
			continue;

		tri_split_prepare(list, best->p);
		tri_split();

		/* lista de tras */
		if (tri_split_parts[0])
		{
			(*pback) = tri_split_parts[0];
			pback = &(*pback)->next;
		}

		/* lista do plano -> lista da frente */
		if (tri_split_parts[1])
		{
			(*pfront) = tri_split_parts[1];
			pfront = &(*pfront)->next;
		}

		/* lista da frente */
		if (tri_split_parts[2])
		{
			(*pfront) = tri_split_parts[1];
			pfront = &(*pfront)->next;
		}
	}

	/* inception */
	node->back = bspc(back);
	node->front = bspc(front);
	return node;
}
