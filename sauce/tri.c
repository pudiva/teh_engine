#include "vec.h"
#include "tri.h"

#include <assert.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

/*
 * coisas públicas
 *
 */
int tri_split_count, tri_split_n_parts[3], tri_split_score;
struct tri* tri_split_parts[3];

#define n_splits tri_split_count
#define n_parts tri_split_n_parts
#define score tri_split_score
#define y tri_split_parts

/*
 * piscina de triangulinhos jajajaja
 *
 */
struct tri tri_pool[TRI_POOL_LEN];
int tri_pool_c = 0;

struct tri* tri_alloc()
{
	assert (tri_pool_c < TRI_POOL_LEN);
	memset(&tri_pool[tri_pool_c], 0, sizeof (struct tri));
	return &tri_pool[tri_pool_c++];
}

void tri_free(struct tri* tri)
{
	/* TODO */
}

/*
 * globalização
 *
 */
static struct tri* x;
static float* p;

static float h[3], v[5][3], tc[5][2], wa, wb;
static int cmp[3];

static int a, b, c;
static struct tri* ya, * yb, * yc;

static int x_side;

#define N_PARTS_WEIGHT 4
#define UNBALANCE_WEIGHT 1

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
void tri_split_prepare(struct tri* src, float* plane)
{
	x = src;
	p = plane;

	h[0] = vec3_dot(x->v[0], p);
	h[1] = vec3_dot(x->v[1], p);
	h[2] = vec3_dot(x->v[2], p);

	cmp[0] = vec_cmp(h[0], p[3]);
	cmp[1] = vec_cmp(h[1], p[3]);
	cmp[2] = vec_cmp(h[2], p[3]);

	n_parts[0] = 0;
	n_parts[1] = 0;
	n_parts[2] = 0;

	n_splits = 0;
	for (a = 0, b = 1, c = 2; a < 3 && n_splits == 0; b = c, c = a++)
	{
		switch (cmp[b] - cmp[a])
		{
		case -2:
		case 2:
			switch (cmp[c] - cmp[b])
			{
			case -1:
			case 1:
				n_parts[0] = 1;
				n_parts[2] = 1;
				n_splits = 1;
				goto BATEU_A_ONDA_FORTE;

			case 0:
				n_parts[cmp[a]+1] = 1;
				n_parts[cmp[b]+1] = 2;
				n_splits = 2;
				goto BATEU_A_ONDA_FORTE;
			}
		}
	}

	/* aqui não bateu a onda forte */
	a = 0;
	switch (cmp[0] + cmp[1] + cmp[2])
	{
		case -3:
		case -2:
		case -1:
			x_side = -1;
			break;

		case 0:
			x_side = 0;
			break;

		case 1:
		case 2:
		case 3:
			x_side = 1;
	}

BATEU_A_ONDA_FORTE:
	n_parts[x_side+1] = 1;

	score = (n_parts[0] + n_parts[1] - n_parts[2]) * UNBALANCE_WEIGHT +
		(n_parts[0] + n_parts[1] + n_parts[2]) * N_PARTS_WEIGHT;
}

/*
 * copia vertices na ordem especificada
 *
 */
static inline void vpick(
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

/*
 * realmente divide a parada
 *
 */
void tri_split(void)
{
	y[0] = NULL;
	y[1] = NULL;
	y[2] = NULL;

	if (n_splits == 0)
	{
		y[x_side+1] = x;
	}

	if (0 < n_splits)
	{
		ya = y[cmp[a]+1] = x;
		yb = y[cmp[b]+1] = tri_alloc();
		wa = (p[3] - h[a]) / (h[b] - h[a]);
		wb = (p[3] - h[c]) / (h[a] - h[c]);

		vpick(v, tc, x->v, x->tc, 5, a, b, c, b, a);
		vec3_lerp(wa, v[0], v[3]);
		vec2_lerp(wa, tc[0], tc[3]);
		vec3_lerp(wb, v[2], v[4]);
		vec2_lerp(wb, tc[2], tc[4]);

		vpick(ya->v, ya->tc, v, tc, 3, 0, 3, 4);
		vpick(yb->v, yb->tc, v, tc, 3, 3, 1, 2);
		vec4_copy(x->p, yb->p);

		/* caso 1 tem 2 triangulos */
		if (n_splits == 2)
		{
			/* caso 2 tem 3 triangulos */
			yc = yb->next = tri_alloc();

			vpick(yc->v, yc->tc, v, tc, 3, 2, 4, 3);
			vec4_copy(x->p, yc->p);
		}
	}
}

