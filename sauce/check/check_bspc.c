#include <stdbool.h>
#include <check.h>
#include <math.h>

#include <stdio.h>

#include "bspc.h"
#include "vec.h"
#include "poly_pool.h"

/*
 * triângulo isóceles circunscrito em circunferência de raio 1 com centro na
 * origem
 *
 */
#define SIN 0.8660253882408142 /* satã */
#define COS 0.5

static struct vert verts[][3];
static struct vert verts_orig[][3] =
{
	{
		{{ 1,	 0,	 0},	{0}, {0}, verts[0]+1},
		{{-COS,	 SIN,	 0},	{0}, {0}, verts[0]+2},
		{{-COS,	-SIN,	 0},	{0}, {0}, verts[0]+0},
	},
	{
		{{ 1,	 0,	 1},	{0}, {0}, verts[1]+1},
		{{-COS,	 SIN,	 1},	{0}, {0}, verts[1]+2},
		{{-COS,	-SIN,	 1},	{0}, {0}, verts[1]+0},
	},
	{
		{{ 0,	 0,	 1},	{0}, {0}, verts[2]+1},
		{{ SIN,	 0,	-COS},	{0}, {0}, verts[2]+2},
		{{-SIN,	 0,	-COS},	{0}, {0}, verts[2]+0},
	},
	{
		{{ 0,	 1,	 1},	{0}, {0}, verts[3]+1},
		{{ SIN,	 1,	-COS},	{0}, {0}, verts[3]+2},
		{{-SIN,	 1,	-COS},	{0}, {0}, verts[3]+0},
	},
};

static struct poly tris_orig[][2] =
{
	{
		{{ 0,  0,  1,  0}, 3, verts[0]},
		{{ 0,  0,  1,  0}, 3, verts[1]},
	},
	{
		{{ 0,  1,  0,  0}, 3, verts[2]},
		{{ 0,  1,  0,  0}, 3, verts[3]},
	},
};

static struct vert verts[sizeof (verts_orig)/sizeof (struct vert[3])][3];
static struct poly tris[sizeof (tris_orig)/sizeof (struct poly[2])][2];

static void setup_tris()
{
	memcpy(tris, tris_orig, sizeof (tris_orig));
	memcpy(verts, verts_orig, sizeof (verts_orig));
	memset(node_pool, 0, sizeof (node_pool));
	node_pool_c = 0;
};

/*
 * combinações de triângulos e translações
 *
 */
struct test_case
{
	struct poly* x[2];
	int n;
};

struct test_case cases[] =
{
	{{tris[0]+0, tris[0]+1}, 5},
	{{tris[1]+0, tris[1]+1}, 5},
	{{tris[0]+0, tris[1]+1}, 7},
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
	tcase_add_checked_fixture(tc_isoceles, setup_tris, NULL);

	tcase_add_loop_test(tc_isoceles, test_bspc, 0, N_CASES);
	suite_add_tcase(s, tc_isoceles);

	return s;
}
