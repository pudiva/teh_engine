#include <stdbool.h>
#include <check.h>

#include "vec.h"

#include "bspc.h"

#include <math.h>

#define PI 3.14159265359
#define A (-0.5)
#define B (0.86602540378)

/* triangulo isoceles */
static struct tri* isoceles(struct tri* x)
{
	int i;

	memset(x, 0, sizeof(struct tri));

	for (i = 0; i < 3; ++i)
	{
		x->v[i][0] = cos((i * 2 * PI)/3);
		x->v[i][1] = sin((i * 2 * PI)/3);
		x->v[i][2] = 0;
		vec2_copy(x->v[i], x->tc[i]);
	}

	x->p[0] = 0;
	x->p[1] = 0;
	x->p[2] = 1;
	x->p[3] = 0;
	return x;
}

/* mediana do ponto a como splitter */
static void mediana_splitter(float* p, struct tri* x, float w, int a, int b, int c)
{
	float m[3];

	vec3_copy(x->v[c], m);
	vec3_lerp(0.5, x->v[b], m);
	vec3_axpy(-1, x->v[a], m);
	vec3_cross(x->p, m, p);
	vec3_normalize(p);
	p[3] = vec3_dot(m, p)*(1-w) + vec3_dot(x->v[c], p)*w;
}

static void test_abc(int a, int b, int c)
{
	float p[4];
	struct tri x;

	/* tudo na frente */
	isoceles(&x);
	mediana_splitter(p, &x, -2, a, b, c);
	tri_split_prepare(&x, p);
	tri_split();

	ck_assert_ptr_null(tri_split_parts[0]);
	ck_assert_ptr_null(tri_split_parts[1]);
	ck_assert_ptr_nonnull(tri_split_parts[2]);
	ck_assert_ptr_null(tri_split_parts[2]->next);

	mediana_splitter(p, &x, -1, a, b, c);
	tri_split_prepare(&x, p);
	tri_split();

	ck_assert_ptr_null(tri_split_parts[0]);
	ck_assert_ptr_null(tri_split_parts[1]);
	ck_assert_ptr_nonnull(tri_split_parts[2]);
	ck_assert_ptr_null(tri_split_parts[2]->next);

	/* tudo atras */
	isoceles(&x);
	mediana_splitter(p, &x, 2, a, b, c);
	tri_split_prepare(&x, p);
	tri_split();

	ck_assert_ptr_nonnull(tri_split_parts[0]);
	ck_assert_ptr_null(tri_split_parts[0]->next);
	ck_assert_ptr_null(tri_split_parts[1]);
	ck_assert_ptr_null(tri_split_parts[2]);

	isoceles(&x);
	mediana_splitter(p, &x, 1, a, b, c);
	tri_split_prepare(&x, p);
	tri_split();

	ck_assert_ptr_nonnull(tri_split_parts[0]);
	ck_assert_ptr_null(tri_split_parts[0]->next);
	ck_assert_ptr_null(tri_split_parts[1]);
	ck_assert_ptr_null(tri_split_parts[2]);

	/* divide em 2 */
	isoceles(&x);
	mediana_splitter(p, &x, 0, a, b, c);
	tri_split_prepare(&x, p);
	tri_split();

	ck_assert_ptr_nonnull(tri_split_parts[0]);
	ck_assert_ptr_null(tri_split_parts[0]->next);
	ck_assert_ptr_null(tri_split_parts[1]);
	ck_assert_ptr_nonnull(tri_split_parts[2]);
	ck_assert_ptr_null(tri_split_parts[2]->next);

	/* divide em 3 com 2 do lado da frente */
	isoceles(&x);
	mediana_splitter(p, &x, -0.5, a, b, c);
	tri_split_prepare(&x, p);
	tri_split();

	ck_assert_ptr_nonnull(tri_split_parts[0]);
	ck_assert_ptr_null(tri_split_parts[0]->next);
	ck_assert_ptr_null(tri_split_parts[1]);
	ck_assert_ptr_nonnull(tri_split_parts[2]);
	ck_assert_ptr_nonnull(tri_split_parts[2]->next);

	/* divide em 3 com 2 do lado de tras */
	isoceles(&x);
	mediana_splitter(p, &x, 0.5, a, b, c);
	tri_split_prepare(&x, p);
	tri_split();

	ck_assert_ptr_nonnull(tri_split_parts[0]);
	ck_assert_ptr_nonnull(tri_split_parts[0]->next);
	ck_assert_ptr_null(tri_split_parts[1]);
	ck_assert_ptr_nonnull(tri_split_parts[2]);
	ck_assert_ptr_null(tri_split_parts[2]->next);
}

START_TEST(test_tri_split)
{
	test_abc(0, 1, 2);
	test_abc(1, 2, 0);
	test_abc(2, 0, 1);
}
END_TEST

Suite* teh_bsp_suite()
{
	Suite* s;
	TCase* tc_tri_split;

	s = suite_create("teh_bsp");

	tc_tri_split = tcase_create("tri_split");

	tcase_add_test(tc_tri_split, test_tri_split);
	suite_add_tcase(s, tc_tri_split);

	return s;
}

int main(int argc, char* argv[])
{
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = teh_bsp_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? 0 : -1;
}
