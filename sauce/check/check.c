#include <stdbool.h>
#include <check.h>

#include "vec.h"
#include "math.h"

#include "teh_bspc_lib.h"

#define PI 3.14159265359
#define A (-0.5)
#define B (0.86602540378)

static struct tri* isoceles(struct tri* x, const float* c)
{
	int i;

	memset(x, 0, sizeof(*x));

	for (i = 0; i < 3; ++i)
	{
		x->v[i][0] = c[0] + cos((i * 2 * PI)/3);
		x->v[i][1] = c[1] + sin((i * 2 * PI)/3);
		x->v[i][2] = c[2] + 0;
		vec2_copy(x->v[i], x->tc[i]);
	}

	x->plane[0] = 0;
	x->plane[1] = 0;
	x->plane[2] = 1;
	x->plane[0] = 0;
	return x;
}

START_TEST(test_triangle_split)
{
	int a, b, c;
	float plane[4];
	struct tri* x, * y[3];

	x = tri_alloc();

	for (a = 2, b = 0, c = 1; b < 3; c = a, a = b++)
	{
		isoceles(x, zero3);

		vec3_copy(x->v[b], plane);
		vec3_axpy(-1, x->v[a], plane);
		vec3_normalize(plane);

		/* tudo na frente */
		plane[3] = vec3_dot(x->v[a], plane);
		tri_shallow_split(x, y, plane);
		ck_assert_ptr_null(y[0]);
		ck_assert_ptr_null(y[1]);
		ck_assert_ptr_nonnull(y[2]);
		ck_assert_ptr_null(y[2]->next);

		/* tudo atras */
		plane[3] = vec3_dot(x->v[b], plane);
		tri_shallow_split(x, y, plane);
		ck_assert_ptr_nonnull(y[0]);
		ck_assert_ptr_null(y[0]->next);
		ck_assert_ptr_null(y[1]);
		ck_assert_ptr_null(y[2]);

		/* divide em 2 */
		plane[3] = vec3_dot(x->v[c], plane);
		tri_shallow_split(x, y, plane);
		ck_assert_ptr_nonnull(y[0]);
		ck_assert_ptr_null(y[0]->next);
		ck_assert_ptr_null(y[1]);
		ck_assert_ptr_nonnull(y[2]);
		ck_assert_ptr_null(y[2]->next);

		/* divide em 3 com 2 do lado da frente */
		isoceles(x, zero3);
		plane[3] = vec3_dot(x->v[c], plane) - 0.1;
		tri_shallow_split(x, y, plane);
		ck_assert_ptr_nonnull(y[0]);
		ck_assert_ptr_null(y[0]->next);
		ck_assert_ptr_null(y[1]);
		ck_assert_ptr_nonnull(y[2]);
		ck_assert_ptr_nonnull(y[2]->next);

		/* divide em 3 com 2 do lado de tras */
		isoceles(x, zero3);
		plane[3] = vec3_dot(x->v[c], plane) + 0.1;
		tri_shallow_split(x, y, plane);
		ck_assert_ptr_nonnull(y[0]);
		ck_assert_ptr_nonnull(y[0]->next);
		ck_assert_ptr_null(y[1]);
		ck_assert_ptr_nonnull(y[2]);
		ck_assert_ptr_null(y[2]->next);
	}
}
END_TEST

Suite* teh_bsp_suite()
{
	Suite* s;
	TCase* tc_triangle_split;

	s = suite_create("teh_bsp");

	tc_triangle_split = tcase_create("triangle_split");

	tcase_add_test(tc_triangle_split, test_triangle_split);
	suite_add_tcase(s, tc_triangle_split);

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
