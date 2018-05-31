#include <stdbool.h>
#include <check.h>
#include <math.h>

#include "box.h"
#include "relp.h"

static struct box boxxy =
{
	{ 1,  2,  3},
	{ 6, 8, 10},
	{
		{-3, -4, -5},
		{ 3, -4, -5},
		{-3,  4, -5},
		{ 3,  4, -5},
		{-3, -4,  5},
		{ 3, -4,  5},
		{-3,  4,  5},
		{ 3,  4,  5},
	}
};

/* testa as parada */
START_TEST(test_box_new)
{
	struct box* b;

	b = box_new(boxxy.pos, boxxy.size);

	assert_vec3_eq(b->pos, boxxy.pos);
	assert_vec3_eq(b->size, boxxy.size);
	assert_vec3_eq(b->verts[0], boxxy.verts[0]);
	assert_vec3_eq(b->verts[1], boxxy.verts[1]);
	assert_vec3_eq(b->verts[2], boxxy.verts[2]);
	assert_vec3_eq(b->verts[3], boxxy.verts[3]);
	assert_vec3_eq(b->verts[4], boxxy.verts[4]);
	assert_vec3_eq(b->verts[5], boxxy.verts[5]);
	assert_vec3_eq(b->verts[6], boxxy.verts[6]);
	assert_vec3_eq(b->verts[7], boxxy.verts[7]);
}
END_TEST

static void setup_pools()
{
	/* NOTE: issos são macros, por isso uma função em volta */
	box_pool_init();
}

/* teeeeeste */
Suite* box_suite()
{
	Suite* s;
	TCase* tc_boxxy;

	s = suite_create("box");
	tc_boxxy = tcase_create("boxxy");
	tcase_add_unchecked_fixture(tc_boxxy, setup_pools, NULL);

	tcase_add_test(tc_boxxy, test_box_new);

	suite_add_tcase(s, tc_boxxy);

	return s;
}
