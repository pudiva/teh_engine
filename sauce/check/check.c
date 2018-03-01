#include <check.h>

Suite* poly_suite();
Suite* tri_split_suite();
Suite* bspc_suite();

int main(int argc, char* argv[])
{
	int number_failed;
	SRunner *sr;

	sr = srunner_create(tri_split_suite());
	srunner_add_suite(sr, poly_suite());
	srunner_add_suite(sr, bspc_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? 0 : -1;
}
