#include <check.h>

Suite* tri_split_suite();

int main(int argc, char* argv[])
{
	int number_failed;
	SRunner *sr;

	sr = srunner_create(tri_split_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? 0 : -1;
}
