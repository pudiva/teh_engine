#include <stdbool.h>
#include <check.h>
#include <math.h>

#include "bspc.h"

#define N_CASES 1

/* testa as parada */
START_TEST(test_bspc)
{
}
END_TEST

/* teeeeeste */
Suite* bspc_suite()
{
	Suite* s;
	TCase* tc_FIXME;

	s = suite_create("bspc");
	tc_FIXME = tcase_create("FIXME");

	tcase_add_loop_test(tc_FIXME, test_bspc, 0, N_CASES);
	suite_add_tcase(s, tc_FIXME);

	return s;
}
