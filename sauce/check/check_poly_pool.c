#include <stdbool.h>
#include <check.h>
#include <math.h>

#include "poly_pool.h"

/*
 * verts
 *
 */

/* aloca todos os vertices */
START_TEST(test_vert_alloc_all)
{
	int i;

	poly_pool_init();
	for (i = 0; i < VERT_POOL_SIZE; ++i)
		ck_assert_ptr_nonnull(vert_alloc());
}
END_TEST

/* aloca todos os vértices, desaloca dois e aloca de novo */
START_TEST(test_vert_alloc_all_free_free_alloc_alloc)
{
	int i;
	struct vert* v1, * v2;

	poly_pool_init();

	vert_alloc();
	v1 = vert_alloc();
	v2 = vert_alloc();

	for (i = 3; i < VERT_POOL_SIZE; ++i)
		vert_alloc();

	vert_free(v1);
	vert_free(v2);
	ck_assert_ptr_eq(v2, vert_alloc());
	ck_assert_ptr_eq(v1, vert_alloc());
}
END_TEST

/*
 * polys
 *
 */

/* aloca todos os polígonos */
START_TEST(test_poly_alloc_all)
{
	int i;

	poly_pool_init();
	for (i = 0; i < POLY_POOL_SIZE; ++i)
		ck_assert_ptr_nonnull(poly_alloc());
}
END_TEST

/* aloca todos os polígonos, desaloca dois e aloca de novo */
START_TEST(test_poly_alloc_all_free_free_alloc_alloc)
{
	int i;
	struct poly* poly1, * poly2;

	poly_pool_init();

	poly_alloc();
	poly1 = poly_alloc();
	poly2 = poly_alloc();

	for (i = 3; i < POLY_POOL_SIZE; ++i)
		poly_alloc();

	poly_free(poly1);
	poly_free(poly2);
	ck_assert_ptr_eq(poly2, poly_alloc());
	ck_assert_ptr_eq(poly1, poly_alloc());
}
END_TEST

/* teeeeeste */
Suite* poly_pool_suite()
{
	Suite* s;
	TCase* tc_core;

	s = suite_create("poly_pool");
	tc_core = tcase_create("core");

	tcase_add_test(tc_core, test_vert_alloc_all);
	tcase_add_test(tc_core, test_vert_alloc_all_free_free_alloc_alloc);

	tcase_add_test(tc_core, test_poly_alloc_all);
	tcase_add_test(tc_core, test_poly_alloc_all_free_free_alloc_alloc);

	suite_add_tcase(s, tc_core);

	return s;
}
