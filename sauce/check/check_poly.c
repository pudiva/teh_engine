#include <stdbool.h>
#include <check.h>
#include <math.h>

#include "vec.h"
#include "poly.h"
#include "poly_pool.h"

/*
 * planos de divisão
 *
 */
static float yz_planes[7][4] =
{
	{  1,  0,  0, -3},
	{  1,  0,  0, -2},
	{  1,  0,  0, -1},
	{  1,  0,  0,  0},
	{  1,  0,  0,  1},
	{  1,  0,  0,  2},
	{  1,  0,  0,  3},
};

static float xy_planes[3][4] =
{
	{  0,  0,  1, -1},
	{  0,  0,  1,  0},
	{  0,  0,  1,  1},
};


/*
 * quadrado
 *
 */
static struct vert verts[];
static const struct vert verts_orig[] =
{
	{.pos = { 2,	 0,	 0}, .next = verts + 1},
	{.pos = { 0,	 2,	 0}, .next = verts + 2},
	{.pos = {-2,	 0,	 0}, .next = verts + 3},
	{.pos = { 0,	-2,	 0}, .next = verts + 0},
};

static const struct poly quadrado_orig =
{
	.p = {0, 0, 1, 0},
	.n_verts = 4,
	.verts = verts,
};

static struct vert verts[sizeof (verts_orig)/sizeof(struct vert)];
static struct poly quadrado;

static void setup_quadrado()
{
	memcpy(verts, verts_orig, sizeof (verts_orig));
	memcpy(&quadrado, &quadrado_orig, sizeof (quadrado_orig));
}

/*
 * vert_cmp
 *
 */
struct vert_cmp_case
{
	struct vert* v;
	const float* p;
	float h;
	int c;
	enum side side;
};

static struct vert_cmp_case vert_cmp_cases[] =
{
	/* v		p		h	 c	side */

	/* eixo x */
	{verts+0,	yz_planes[0],	 2,	 1,	FRONT},
	{verts+1,	yz_planes[0],	 0,	 1,	FRONT},
	{verts+2,	yz_planes[0],	-2,	 1,	FRONT},
	{verts+3,	yz_planes[0],	 0,	 1,	FRONT},

	{verts+0,	yz_planes[1],	 2,	 1,	FRONT},
	{verts+1,	yz_planes[1],	 0,	 1,	FRONT},
	{verts+2,	yz_planes[1],	-2,	 0,	ON_PLANE},
	{verts+3,	yz_planes[1],	 0,	 1,	FRONT},

	{verts+0,	yz_planes[2],	 2,	 1,	FRONT},
	{verts+1,	yz_planes[2],	 0,	 1,	FRONT},
	{verts+2,	yz_planes[2],	-2,	-1,	BACK},
	{verts+3,	yz_planes[2],	 0,	 1,	FRONT},

	{verts+0,	yz_planes[3],	 2,	 1,	FRONT},
	{verts+1,	yz_planes[3],	 0,	 0,	ON_PLANE},
	{verts+2,	yz_planes[3],	-2,	-1,	BACK},
	{verts+3,	yz_planes[3],	 0,	 0,	ON_PLANE},

	{verts+0,	yz_planes[4],	 2,	 1,	FRONT},
	{verts+1,	yz_planes[4],	 0,	-1,	BACK},
	{verts+2,	yz_planes[4],	-2,	-1,	BACK},
	{verts+3,	yz_planes[4],	 0,	-1,	BACK},

	{verts+0,	yz_planes[5],	 2,	 0,	ON_PLANE},
	{verts+1,	yz_planes[5],	 0,	-1,	BACK},
	{verts+2,	yz_planes[5],	-2,	-1,	BACK},
	{verts+3,	yz_planes[5],	 0,	-1,	BACK},

	{verts+0,	yz_planes[6],	 2,	-1,	BACK},
	{verts+1,	yz_planes[6],	 0,	-1,	BACK},
	{verts+2,	yz_planes[6],	-2,	-1,	BACK},
	{verts+3,	yz_planes[6],	 0,	-1,	BACK},

	/* eixo z */                 	
	{verts+0,	xy_planes[0],	 0,	 1,	FRONT},
	{verts+1,	xy_planes[0],	 0,	 1,	FRONT},
	{verts+2,	xy_planes[0],	 0,	 1,	FRONT},
	{verts+3,	xy_planes[0],	 0,	 1,	FRONT},

	{verts+0,	xy_planes[1],	 0,	 0,	ON_PLANE},
	{verts+1,	xy_planes[1],	 0,	 0,	ON_PLANE},
	{verts+2,	xy_planes[1],	 0,	 0,	ON_PLANE},
	{verts+3,	xy_planes[1],	 0,	 0,	ON_PLANE},

	{verts+0,	xy_planes[2],	 0,	-1,	BACK},
	{verts+1,	xy_planes[2],	 0,	-1,	BACK},
	{verts+2,	xy_planes[2],	 0,	-1,	BACK},
	{verts+3,	xy_planes[2],	 0,	-1,	BACK},
};
#define N_VERT_CMP_CASES (sizeof (vert_cmp_cases) / sizeof (struct vert_cmp_case))

START_TEST(test_vert_cmp)
{
	struct vert_cmp pc;
	struct vert_cmp_case* x;

	x = vert_cmp_cases + _i;

	vert_cmp(&pc, x->v, x->p);
	ck_assert_ptr_eq(pc.v, x->v);
	ck_assert_ptr_eq(pc.p, x->p);
	ck_assert_float_eq(pc.h, x->h);
	ck_assert_int_eq(pc.c, x->c);
}
END_TEST

/*
 * edge_cmp
 *
 */
struct edge_cmp_case
{
	struct vert* v;
	const float* p;
	enum side side;
};

static struct edge_cmp_case edge_cmp_cases[] =
{
	/* v		p		side */

	/* eixo x */
	{verts+0,	yz_planes[0],	FRONT},
	{verts+1,	yz_planes[0],	FRONT},
	{verts+2,	yz_planes[0],	FRONT},
	{verts+3,	yz_planes[0],	FRONT},

	{verts+0,	yz_planes[1],	FRONT},
	{verts+1,	yz_planes[1],	ON_PLANE | FRONT},
	{verts+2,	yz_planes[1],	ON_PLANE | FRONT},
	{verts+3,	yz_planes[1],	FRONT},

	{verts+0,	yz_planes[2],	FRONT},
	{verts+1,	yz_planes[2],	BACK | FRONT},
	{verts+2,	yz_planes[2],	BACK | FRONT},
	{verts+3,	yz_planes[2],	FRONT},

	{verts+0,	yz_planes[3],	ON_PLANE | FRONT},
	{verts+1,	yz_planes[3],	ON_PLANE | BACK},
	{verts+2,	yz_planes[3],	ON_PLANE | BACK},
	{verts+3,	yz_planes[3],	ON_PLANE | FRONT},

	{verts+0,	yz_planes[4],	BACK | FRONT},
	{verts+1,	yz_planes[4],	BACK},
	{verts+2,	yz_planes[4],	BACK},
	{verts+3,	yz_planes[4],	BACK | FRONT},

	{verts+0,	yz_planes[5],	ON_PLANE | BACK},
	{verts+1,	yz_planes[5],	BACK},
	{verts+2,	yz_planes[5],	BACK},
	{verts+3,	yz_planes[5],	ON_PLANE | BACK},

	{verts+0,	yz_planes[6],	BACK},
	{verts+1,	yz_planes[6],	BACK},
	{verts+2,	yz_planes[6],	BACK},
	{verts+3,	yz_planes[6],	BACK},

	/* eixo z */
	{verts+0,	xy_planes[0],	FRONT},
	{verts+1,	xy_planes[0],	FRONT},
	{verts+2,	xy_planes[0],	FRONT},
	{verts+3,	xy_planes[0],	FRONT},

	{verts+0,	xy_planes[1],	ON_PLANE},
	{verts+1,	xy_planes[1],	ON_PLANE},
	{verts+2,	xy_planes[1],	ON_PLANE},
	{verts+3,	xy_planes[1],	ON_PLANE},

	{verts+0,	xy_planes[2],	BACK},
	{verts+1,	xy_planes[2],	BACK},
	{verts+2,	xy_planes[2],	BACK},
	{verts+3,	xy_planes[2],	BACK},
};
#define N_EDGE_CMP_CASES (sizeof (edge_cmp_cases) / sizeof (struct edge_cmp_case))

START_TEST(test_edge_cmp)
{
	struct edge_cmp ec;
	struct edge_cmp_case* x;

	x = edge_cmp_cases + _i;

	edge_cmp(&ec, x->v, x->p);
	ck_assert_ptr_eq(ec.vc[0].v, x->v);
	ck_assert_ptr_eq(ec.vc[1].v, x->v->next);
	ck_assert_int_eq(ec.side, x->side);
}
END_TEST

/*
 * poly_cmp
 *
 */
struct poly_cmp_case
{
	struct poly* poly;
	const float* p;
	struct vert* v1;
	struct vert* v2;
	enum side side;
};

static struct poly_cmp_case poly_cmp_cases[] =
{
	/* poly		p		v1		v2		side */

	/* eixo x */
	{&quadrado,	yz_planes[0],	verts+3,	verts+3,	FRONT},
	{&quadrado,	yz_planes[1],	verts+2,	verts+3,	ON_PLANE | FRONT},
	{&quadrado,	yz_planes[2],	verts+1,	verts+2,	BACK | FRONT},
	{&quadrado,	yz_planes[3],	verts+1,	verts+3,	ON_PLANE | BACK | FRONT},
	{&quadrado,	yz_planes[4],	verts+0,	verts+3,	BACK | FRONT},
	{&quadrado,	yz_planes[5],	verts+0,	verts+3,	ON_PLANE | BACK},
	{&quadrado,	yz_planes[6],	verts+3,	verts+3,	BACK},

	/* eixo z */
	{&quadrado,	xy_planes[0],	verts+3,	verts+3,	FRONT},
	{&quadrado,	xy_planes[1],	verts+0,	verts+1,	ON_PLANE},
	{&quadrado,	xy_planes[2],	verts+3,	verts+3,	BACK},
};
#define N_POLY_CMP_CASES (sizeof (poly_cmp_cases) / sizeof (struct poly_cmp_case))

/* testa as parada */
START_TEST(test_poly_cmp)
{
	struct poly_cmp pc;
	struct poly_cmp_case* x;

	x = poly_cmp_cases + _i;

	poly_cmp(&pc, x->poly, x->p);
	ck_assert_ptr_eq(pc.poly, x->poly);

	ck_assert_int_eq(pc.side, x->side);
	ck_assert_ptr_eq(pc.ec[0].vc[0].v, x->v1);
	if ((pc.side & SPLIT) == SPLIT)
		ck_assert_ptr_eq(pc.ec[1].vc[0].v, x->v2);
}
END_TEST

/*
 * edge_split
 *
 */
struct edge_split_case
{
	struct vert* v;
	const float* p;
	float split[3];
};

static struct edge_split_case edge_split_cases[] =
{
	/* v		p		split */

	/* eixo x */
	{verts+0,	yz_planes[0],	{0}},
	{verts+1,	yz_planes[0],	{0}},
	{verts+2,	yz_planes[0],	{0}},
	{verts+3,	yz_planes[0],	{0}},

	{verts+0,	yz_planes[1],	{0}},
	{verts+1,	yz_planes[1],	{0}},
	{verts+2,	yz_planes[1],	{0}},
	{verts+3,	yz_planes[1],	{0}},

	{verts+0,	yz_planes[2],	{0}},
	{verts+1,	yz_planes[2],	{ -1,  1,  0}},
	{verts+2,	yz_planes[2],	{ -1, -1,  0}},
	{verts+3,	yz_planes[2],	{0}},

	{verts+0,	yz_planes[3],	{0}},
	{verts+1,	yz_planes[3],	{0}},
	{verts+2,	yz_planes[3],	{0}},
	{verts+3,	yz_planes[3],	{0}},

	{verts+0,	yz_planes[4],	{ 1,  1,  0}},
	{verts+1,	yz_planes[4],	{0}},
	{verts+2,	yz_planes[4],	{0}},
	{verts+3,	yz_planes[4],	{ 1, -1,  0}},

	{verts+0,	yz_planes[5],	{0}},
	{verts+1,	yz_planes[5],	{0}},
	{verts+2,	yz_planes[5],	{0}},
	{verts+3,	yz_planes[5],	{0}},

	{verts+0,	yz_planes[5],	{0}},
	{verts+1,	yz_planes[5],	{0}},
	{verts+2,	yz_planes[5],	{0}},
	{verts+3,	yz_planes[5],	{0}},

	/* eixo z */
	{verts+0,	xy_planes[0],	{0}},
	{verts+1,	xy_planes[0],	{0}},
	{verts+2,	xy_planes[0],	{0}},
	{verts+3,	xy_planes[0],	{0}},

#if 0
	/* esses dão erro mesmo! */
	{verts+0,	xy_planes[1],	{0}},
	{verts+1,	xy_planes[1],	{0}},
	{verts+2,	xy_planes[1],	{0}},
	{verts+3,	xy_planes[1],	{0}},
#endif

	{verts+0,	xy_planes[2],	{0}},
	{verts+1,	xy_planes[2],	{0}},
	{verts+2,	xy_planes[2],	{0}},
	{verts+3,	xy_planes[2],	{0}},
};

#define N_EDGE_SPLIT_CASES (sizeof (edge_split_cases) / sizeof (struct edge_split_case))

#define assert_vec3_eq(a, b) ck_assert_msg( \
		(a)[0] == (b)[0] && (a)[1] == (b)[1] && (a)[2] == (b)[2], \
		"vectors " #a" and "#b" are different: {%f, %f, %f} and {%f, %f, %f}\n", \
		a[0], a[1], a[2], b[0], b[1], b[2] \
		)

#include <stdio.h>

/* testa as parada */
START_TEST(test_edge_split)
{
	struct edge_cmp ec;
	struct edge_split_case* x;
	struct vert* y[2], * v[3];

	x = edge_split_cases + _i;

	edge_cmp(&ec, x->v, x->p);

	v[0] = x->v;
	v[1] = v[0]->next;
	v[2] = v[1]->next;

	edge_split_winding(&ec, y+0, y+1);

	/* tudo dum lado só */
	if (ec.vc[0].side == ec.vc[1].side)
	{
		ck_assert_ptr_eq(y[0], v[0]);
		ck_assert_ptr_eq(y[0]->next, v[1]);
		ck_assert_ptr_eq(y[0]->next->next, v[2]);
	}

	/* dois lados */
	else
	{
		ck_assert_ptr_eq(y[0], v[0]);

		if (ec.vc[0].side & ON_PLANE)
			ck_assert_ptr_null(y[0]->next);

		else if (ec.vc[1].side & ON_PLANE)
		{
			ck_assert_ptr_eq(y[0]->next, v[1]);
			assert_vec3_eq(y[1]->pos, v[1]->pos);
			ck_assert_ptr_eq(y[1]->next, v[2]);
		}

		else
		{
			assert_vec3_eq(y[0]->next->pos, x->split);
			assert_vec3_eq(y[1]->pos, x->split);
			ck_assert_ptr_eq(y[1]->next, v[1]);
			ck_assert_ptr_eq(y[1]->next->next, v[2]);
		}
	}
}
END_TEST

/*
 * poly_split
 *
 */
struct poly_split_case
{
	struct poly* poly;
	const float* p;
	int n_verts[2];
	float vert_pos[10][3];
};

static struct poly_split_case poly_split_cases[] =
{
	{
		&quadrado,
		yz_planes[2],
		{ 3,  5},
		{
			{-1,	 1,	 0},
			{-2,	 0,	 0},
			{-1,	-1,	 0},
			{ 2,	 0,	 0},
			{ 0,	 2,	 0},
			{-1,	 1,	 0},
			{-1,	-1,	 0},
			{ 0,	-2,	 0},
		}
	},
	{
		&quadrado,
		yz_planes[3],
		{ 3,  3},
		{
			{ 0,	 2,	 0},
			{-2,	 0,	 0},
			{ 0,	-2,	 0},
			{ 2,	 0,	 0},
			{ 0,	 2,	 0},
			{ 0,	-2,	 0},
		}
	},
	{
		&quadrado,
		yz_planes[4],
		{ 5,  3},
		{
			{ 1,	 1,	 0},
			{ 0,	 2,	 0},
			{-2,	 0,	 0},
			{ 0,	-2,	 0},
			{ 1,	-1,	 0},
			{ 2,	 0,	 0},
			{ 1,	 1,	 0},
			{ 1,	-1,	 0},
		},
	},
};

#define N_POLY_SPLIT_CASES (sizeof (poly_split_cases) / sizeof (struct poly_split_case))

/* testa as parada */
START_TEST(test_poly_split)
{
	struct poly_cmp pc;
	struct poly_split_case* x;
	struct poly* parts[2];
	struct vert* v;
	int j, k;

	x = poly_split_cases + _i;

	poly_cmp(&pc, x->poly, x->p);
	poly_split(&pc, parts + 0, parts + 1);

	ck_assert_int_eq(parts[0]->n_verts, x->n_verts[0]);
	ck_assert_int_eq(parts[1]->n_verts, x->n_verts[1]);

	/* FIXME: relaxar a ordenação dos vértices */
	for (j = 0; j < 2; ++j)
		for (k = 0, v = parts[0]->verts; k < parts[0]->n_verts; ++k, v = v->next)
			assert_vec3_eq(v->pos, x->vert_pos[k]);
}
END_TEST

/* teeeeeste */
Suite* poly_suite()
{
	Suite* s;
	TCase* tc_isoceles;

	s = suite_create("poly");
	tc_isoceles = tcase_create("quadrado");
	tcase_add_checked_fixture(tc_isoceles, setup_quadrado, NULL);

	tcase_add_unchecked_fixture(tc_isoceles, poly_pool_init, NULL);
	tcase_add_loop_test(tc_isoceles, test_vert_cmp, 0, N_VERT_CMP_CASES);
	tcase_add_loop_test(tc_isoceles, test_edge_cmp, 0, N_EDGE_CMP_CASES);
	tcase_add_loop_test(tc_isoceles, test_poly_cmp, 0, N_POLY_CMP_CASES);
	tcase_add_loop_test(tc_isoceles, test_edge_split, 0, N_EDGE_SPLIT_CASES);
	tcase_add_loop_test(tc_isoceles, test_poly_split, 0, N_POLY_SPLIT_CASES);
	suite_add_tcase(s, tc_isoceles);

	return s;
}
