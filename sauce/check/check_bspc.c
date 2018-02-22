#include <stdbool.h>
#include <check.h>
#include <math.h>

#include <stdio.h>

#include "bspc.h"
#include "vec.h"

/*
 * triângulo isóceles circunscrito em circunferência de raio 1 com centro na
 * origem
 *
 */
#define SIN 0.8660253882408142 /* satã */
#define COS 0.5
static struct tri iso_xy =
{
	.v =
	{
		{ 1,	 0,	 0},
		{-COS,	 SIN,	 0},
		{-COS,	-SIN,	 0}
	},
	.tc =
	{
		{ 1,	 0},
		{-COS,	 SIN},
		{-COS,	-SIN}
	},
	.p = {0, 0, 1, 0},
	.next = NULL
};

static struct tri iso_zx =
{
	.v =
	{
		{ 0,	 0,	 1},
		{ SIN,	 0,	-COS},
		{-SIN,	 0,	-COS}
	},
	.tc =
	{
		{ 1,	 0},
		{-COS,	 SIN},
		{-COS,	-SIN}
	},
	.p = {0, 1, 0, 0},
	.next = NULL
};

/*
 * combinações de triângulos e translações
 *
 */
struct test_case
{
	struct tri* x[2];
	float t[2][3];
	int n;
};

struct test_case cases[] =
{
	{.x = {&iso_xy, &iso_xy}, .t = {{0, 0, 0}, {0, 0, 1}}, .n=5},
	{.x = {&iso_zx, &iso_zx}, .t = {{0, 0, 0}, {0, 1, 0}}, .n=5},

	{.x = {&iso_xy, &iso_zx}, .t = {{0, 0, 0}, {0, 0, 0}}, .n=7},
};
#define N_CASES (sizeof (cases) / sizeof (struct test_case))

/* testa as parada */
START_TEST(test_bspc)
{
	int j, k;
	struct tri* tris[2];
	struct test_case* tc;

	tc = &cases[_i];

	for (j = 0; j < 2; ++j)
	{
		tris[j] = tri_alloc();
		memcpy(tris[j], tc->x[j], sizeof (struct tri));

		for (k = 0; k < 3; ++k)
			vec3_axpy(1, tc->t[j], tris[j]->v[k]);

		tris[j]->p[3] = vec3_dot(tris[j]->v[j], tris[j]->p);
	}

	for (j = 0; j < 1; ++j)
		tris[j]->next = tris[j+1];

	bspc(tris[0]);
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

	tcase_add_loop_test(tc_isoceles, test_bspc, 0, N_CASES);
	suite_add_tcase(s, tc_isoceles);

	return s;
}
