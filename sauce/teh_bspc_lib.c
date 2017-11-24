/*
 * referencias:
 *
 * http://www.cs.utah.edu/~jsnider/SeniorProj/BSP1/default.html
 * https://github.com/id-Software/Quake-III-Arena
 *
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>

#include "teh_model.h"
#include "vec.h"

#include "teh_bspc_lib.h"

/*
 * piscina de triangulos jajaja
 *
 */
struct tri tri_pool[TRI_POOL_LEN];
unsigned long tri_pool_c = 0;

struct tri* tri_alloc()
{
	memset(&tri_pool[tri_pool_c], 0, sizeof (struct tri));
	return &tri_pool[tri_pool_c++];
}

/*
 * FIXME: TA ERRADO
 * custo da divisão
 *
 */
int tri_shallow_split_score(struct tri* restrict x, float* restrict plane)
{
	float h[3];
	int cmp[3], a, b, n;

	h[0] = vec3_dot(x->v[0], plane);
	h[1] = vec3_dot(x->v[1], plane);
	h[2] = vec3_dot(x->v[2], plane);

	cmp[0] = vec_cmp(h[0], plane[3]);
	cmp[1] = vec_cmp(h[1], plane[3]);
	cmp[2] = vec_cmp(h[2], plane[3]);

	n = 0;
	for (a = 2, b = 0; b < 3; a = b++)
	{
		switch (cmp[b]-cmp[a])
		{
		case -2:
		case 2:
			++n;
		}
	}

	return n == 0 ? 1 : n*8;
}

int tri_deep_split_score(struct tri* restrict x, float* restrict plane)
{
	struct tri* cur;
	int s;

	s = 0;
	for (cur = x; cur; cur = cur->next)
		s += tri_shallow_split_score(x, plane);

	return s;
}

/*
 * divide triangulos
 *
 */
static void vpick(
		float (*d_v)[3],
		float (*d_tc)[2],
		float (*s_v)[3],
		float (*s_tc)[2],
		int n,
		...
		)
{
	int i, j;
	va_list args;
	va_start(args, n);

	for (i = 0; i < n; ++i)
	{
		j = va_arg(args, int);
		vec3_copy(s_v[j], d_v[i]);
		vec2_copy(s_tc[j], d_tc[i]);
	}
}

void tri_shallow_split(
		struct tri* restrict x,
		struct tri* restrict * restrict y,
		float* restrict plane
	     )
{
	float h[3], v[5][3], tc[5][2], wa, wb;
	struct tri* ya, * yb, * yc;
	int cmp[3], a, b, c;

	y[0] = NULL;
	y[1] = NULL;
	y[2] = NULL;

	h[0] = vec3_dot(x->v[0], plane);
	h[1] = vec3_dot(x->v[1], plane);
	h[2] = vec3_dot(x->v[2], plane);

	cmp[0] = vec_cmp(h[0], plane[3]);
	cmp[1] = vec_cmp(h[1], plane[3]);
	cmp[2] = vec_cmp(h[2], plane[3]);

	for (a = 0, b = 1, c = 2; a < 3; b = c, c = a++)
	{
		switch (cmp[b]-cmp[a])
		{
		case -2:
		case 2:
			switch (cmp[c]-cmp[b])
			{
			case 0:
			case -1:
			case 1:
				goto HIT;
			}
		}
	}

	/* sem colisão */
	for (a = 0; a < 2; ++a)
		if (cmp[a] != 0)
			break;

	y[cmp[a]+1] = x;
	return;

HIT:
	ya = y[cmp[a]+1] = x;
	yb = y[cmp[b]+1] = tri_alloc();
	wa = (plane[3] - h[a]) / (h[b] - h[a]);
	wb = (plane[3] - h[c]) / (h[a] - h[c]);

	/* 
	 * caso 1:      caso 2:
	 *
	 *     c c            c
	 *    /| |\           /\
	 *   / | | \      /| |  \
	 *  /__| |__\    /_| |___\
	 * a         b  a        b
	 *
	 */
	vpick(v, tc, x->v, x->tc, 5, a, b, c, b, a);
	vec3_lerp(wa, v[0], v[3]);
	vec2_lerp(wa, tc[0], tc[3]);
	vec3_lerp(wb, v[2], v[4]);
	vec2_lerp(wb, tc[2], tc[4]);

	vpick(ya->v, ya->tc, v, tc, 3, 0, 3, 4);
	vpick(yb->v, yb->tc, v, tc, 3, 3, 1, 2);
	vec4_copy(x->plane, yb->plane);

	/* caso 1 tem 2 triangulos */
	if (cmp[c] == 0)
		return;

	/* caso 2 tem 3 triangulos */
	yc = yb->next = tri_alloc();

	vpick(yc->v, yc->tc, v, tc, 3, 2, 4, 3);
	vec4_copy(x->plane, yc->plane);
}

void tri_deep_split(
		struct tri* restrict x,
		struct tri* restrict * restrict y,
		float* restrict plane
	      )
{
	struct tri* cur, * next;
	struct tri* top[3];
	int i;

	top[0] = y[0] = NULL;
	top[1] = y[1] = NULL;
	top[2] = y[2] = NULL;

	for (cur = x, next = x->next; cur; cur = next)
	{
		tri_shallow_split(cur, top, plane);

		for (i = 0; i < 3; ++i)
		{
			if (top[i])
			{
				/* y é a base */
				if (y[i] == NULL)
					y[i] = top[i];

				top[i] = top[i]->next;
			}
		}
	}
}

/*
 * constroi nó
 *
 */
int node_init(union node* node, struct tri* x)
{
	struct tri* cur, * best, * y[3];
	int curs, bests;

	bests = INT_MAX;
	for (cur = x; cur; cur = cur->next)
	{
		curs = tri_deep_split_score(x, cur->plane);

		if (curs < bests)
		{
			bests = curs;
			best = cur;
		}
	}

	tri_deep_split(x, y, x->plane);

	/* TODO so p ignora o warning */
	best = x;
	x = best;

	return 0;
}
