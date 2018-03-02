#include <stdlib.h>
#include <assert.h>
#include "poly_pool.h"

static struct poly poly_pool[POLY_POOL_SIZE] = {0};
static struct vert vert_pool[VERT_POOL_SIZE] = {0};

static int n_polys = 0;
static int n_verts = 0;

static struct poly* free_polys = NULL;
static struct vert* free_verts = NULL;

struct poly* poly_alloc()
{
	struct poly* poly;

	poly = free_polys;
	assert (poly);

	free_polys = poly->next;
	++n_polys;
	return poly;
}

void poly_free(struct poly* poly)
{
	if (poly)
	{
		poly->next = free_polys;
		free_polys = poly;
		--n_polys;
	}
}

struct vert* vert_alloc()
{
	struct vert* v;

	v = free_verts;
	assert (v);

	free_verts = v->next;
	++n_verts;
	return v;
}

void vert_free(struct vert* v)
{
	if (v)
	{
		v->next = free_verts;
		free_verts = v;
		--n_verts;
	}
}

static void free_all_polys()
{
	int i;

	for (i = 0; i < POLY_POOL_SIZE; ++i)
		poly_pool[i].next = poly_pool + i + 1;

	poly_pool[i-1].next = NULL;
	free_polys = poly_pool;
	n_polys = 0;
}

static void free_all_verts()
{
	int i;

	for (i = 0; i < VERT_POOL_SIZE; ++i)
		vert_pool[i].next = vert_pool + i + 1;

	vert_pool[i-1].next = NULL;
	free_verts = vert_pool;
	n_verts = 0;
}

void poly_pool_init()
{
	free_all_polys();
	free_all_verts();
}
