#include <stdbool.h>
#include <check.h>
#include <math.h>

#include <stdio.h>

#include "bspc.h"
#include "vec.h"
#include "poly_pool.h"

/*
 * quadradão
 *
 */
static struct vert verts[2][2][4];
static struct vert verts_orig[2][2][4] =
{
	{
		{
			{{ 2,  0,  0}, {0}, {0}, verts[0][0] + 1},
			{{ 0,  2,  0}, {0}, {0}, verts[0][0] + 2},
			{{-2,  0,  0}, {0}, {0}, verts[0][0] + 3},
			{{ 0, -2,  0}, {0}, {0}, verts[0][0] + 0},
		},
		{
			{{ 2,  0,  1}, {0}, {0}, verts[0][1] + 1},
			{{ 0,  2,  1}, {0}, {0}, verts[0][1] + 2},
			{{-2,  0,  1}, {0}, {0}, verts[0][1] + 3},
			{{ 0, -2,  1}, {0}, {0}, verts[0][1] + 0},
		},
	},
	{
		{
			{{ 0,  0,  2}, {0}, {0}, verts[1][0] + 1},
			{{ 0, -2,  0}, {0}, {0}, verts[1][0] + 2},
			{{ 0,  0, -2}, {0}, {0}, verts[1][0] + 3},
			{{ 0,  2,  0}, {0}, {0}, verts[1][0] + 0},
		},
		{
			{{ 1,  0,  2}, {0}, {0}, verts[1][1] + 1},
			{{ 1, -2,  0}, {0}, {0}, verts[1][1] + 2},
			{{ 1,  0, -2}, {0}, {0}, verts[1][1] + 3},
			{{ 1,  2,  0}, {0}, {0}, verts[1][1] + 0},
		},
	},
};

static struct poly polys[2][2];
static struct poly polys_orig[2][2] =
{
	{
		{{ 0,  0,  1,  0}, 4, verts[0][0]},
		{{ 0,  0,  1,  1}, 4, verts[0][1]},
	},
	{
		{{ 1,  0,  0,  0}, 4, verts[1][0]},
		{{ 1,  0,  0,  1}, 4, verts[1][1]},
	},
};

static void setup_polys()
{
	memcpy(polys, polys_orig, sizeof (polys_orig));
	memcpy(verts, verts_orig, sizeof (verts_orig));
	memset(node_pool, 0, sizeof (node_pool));
	node_pool_c = 0;
};

/*
 * combinações de polígonos
 *
 */
struct test_case
{
	struct poly* x[2];
	int n;
};

struct test_case cases[] =
{
	{{polys[0]+0, polys[0]+1}, 5},
	{{polys[1]+0, polys[1]+1}, 5},
	{{polys[0]+0, polys[1]+1}, 7},
};
#define N_CASES (sizeof (cases) / sizeof (struct test_case))

/* testa as parada */
START_TEST(test_bspc)
{
	struct test_case* tc;

	tc = cases + _i;
	tc->x[0]->next = tc->x[1];
	tc->x[0]->next->next = 0;

	ck_assert_ptr_ne(tc->x[0]->next, tc->x[0]);

	behc_node(tc->x[0]);
	//fputs("\n", stderr);
	ck_assert_int_eq(node_pool_c, tc->n);
}
END_TEST

/* teeeeeste */
Suite* bspc_suite()
{
	Suite* s;
	TCase* tc_isoceles;

	s = suite_create("bspc");
	tc_isoceles = tcase_create("isoceles");

	tcase_add_checked_fixture(tc_isoceles, poly_pool_init, NULL);
	tcase_add_checked_fixture(tc_isoceles, setup_polys, NULL);

	tcase_add_loop_test(tc_isoceles, test_bspc, 0, N_CASES);
	suite_add_tcase(s, tc_isoceles);

	return s;
}
