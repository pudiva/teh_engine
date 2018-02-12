#include <stdbool.h>
#include <check.h>
#include <math.h>

#include "vec.h"
#include "tri.h"

/*
 * triângulo isóceles circunscrito em circunferência de raio 1 com centro na
 * origem
 *
 */
#define SIN 0.8660253882408142 /* satã */
#define COS 0.5
static struct tri isoceles =
{
	.v =
	{
		{ 1,	 0,	0},
		{-COS,	 SIN,	0},
		{-COS,	-SIN,	0}
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

/*
 * planos divisores nas 3 medianas mediadoras
 *
 */
#define HOUT	1.732050776481628
#define HIN	0.4330126941204071

struct test_case
{
	float p[4];
	int n[3];
};

static struct test_case cases[] =
{
	{.p = { 0,	 0,	 1,	 0},	.n = {0, 1, 0}},
	{.p = { 0,	 0,	-1,	 0},	.n = {0, 1, 0}},
	{.p = { 0,	 0,	-1,	 1},	.n = {1, 0, 0}},
	{.p = { 0,	 0,	-1,	-1},	.n = {0, 0, 1}},
	{.p = { 0,	 0,	 1,	 1},	.n = {1, 0, 0}},
	{.p = { 0,	 0,	 1,	-1},	.n = {0, 0, 1}},

	{.p = { 0,	-1,	 0,	 HOUT},	.n = {1, 0, 0}},
	{.p = { 0,	-1,	 0,	 SIN},	.n = {1, 0, 0}},
	{.p = { 0,	-1,	 0,	-HOUT},	.n = {0, 0, 1}},
	{.p = { 0,	-1,	 0,	-SIN},	.n = {0, 0, 1}},
	{.p = { 0,	-1,	 0,	 0},	.n = {1, 0, 1}},
	{.p = { 0,	-1,	 0,	-HIN},	.n = {1, 0, 2}},
	{.p = { 0,	-1,	 0,	 HIN},	.n = {2, 0, 1}},

	{.p = { SIN,	 0.5,	-0,	 HOUT},	.n = {1, 0, 0}},
	{.p = { SIN,	 0.5,	-0,	 SIN},	.n = {1, 0, 0}},
	{.p = { SIN,	 0.5,	-0,	-HOUT},	.n = {0, 0, 1}},
	{.p = { SIN,	 0.5,	-0,	-SIN},	.n = {0, 0, 1}},
	{.p = { SIN,	 0.5,	-0,	 0},	.n = {1, 0, 1}},
	{.p = { SIN,	 0.5,	-0,	-HIN},	.n = {1, 0, 2}},
	{.p = { SIN,	 0.5,	-0,	 HIN},	.n = {2, 0, 1}},

	{.p = {-SIN,	 0.5,	 0,	 HOUT},	.n = {1, 0, 0}},
	{.p = {-SIN,	 0.5,	 0,	 SIN},	.n = {1, 0, 0}},
	{.p = {-SIN,	 0.5,	 0,	-HOUT},	.n = {0, 0, 1}},
	{.p = {-SIN,	 0.5,	 0,	-SIN},	.n = {0, 0, 1}},
	{.p = {-SIN,	 0.5,	 0,	 0},	.n = {1, 0, 1}},
	{.p = {-SIN,	 0.5,	 0,	-HIN},	.n = {1, 0, 2}},
	{.p = {-SIN,	 0.5,	 0,	 HIN},	.n = {2, 0, 1}},
};
#define N_CASES (sizeof (cases) / sizeof (struct test_case))

/* testa as parada */
START_TEST(test_tri_split)
{
	int j;
	struct test_case* tc;
	tc = &cases[_i];

	tri_split_prepare(&isoceles, cases[_i].p);

	for (j = 0; j < 3; ++j)
		ck_assert_int_eq(tri_split_n_parts[j], tc->n[j]);

	ck_assert(tri_split_count == tc->n[0] + tc->n[1] + tc->n[2] - 1);

	tri_split();

	for (j = 0; j < 3; ++ j)
	{
		if (0 < tc->n[j])
		{
			ck_assert_ptr_nonnull(tri_split_parts[j]);
			if (1 < tc->n[j])
			{
				ck_assert_ptr_nonnull(tri_split_parts[j]->next);
			}
			else
			{
				ck_assert_ptr_null(tri_split_parts[j]->next);
			}
		}
		else
		{
			ck_assert_ptr_null(tri_split_parts[j]);
		}
	}

	/* TODO: testar vertices e texcoords */
}
END_TEST

/* teeeeeste */
Suite* tri_split_suite()
{
	Suite* s;
	TCase* tc_isoceles;

	s = suite_create("tri_split");
	tc_isoceles = tcase_create("isoceles");

	tcase_add_loop_test(tc_isoceles, test_tri_split, 0, N_CASES);
	suite_add_tcase(s, tc_isoceles);

	return s;
}
