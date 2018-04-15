#include <check.h>
#include "pool.h"

#define BUF_LEN 666

struct six
{
	int six[6];
};

struct six buf[BUF_LEN];
static struct pool pool;

/* inicializa piscina */
static void setup()
{
	pool_init(&pool, 666, sizeof (struct six), buf);
}

/* finaliza piscina */
static void teardown()
{
	pool_fini(&pool);
}

/* testa a inicialização da piscina */
START_TEST(test_pool_init)
{
	ck_assert_ptr_eq(pool.buf, buf);
	ck_assert_int_eq(pool.size, sizeof (struct six));
	ck_assert_int_eq(pool.n_memb, BUF_LEN);
	ck_assert_int_eq(pool.n_free, BUF_LEN);
	ck_assert_int_eq(pool.n_used, 0);
	ck_assert_int_eq(pool.n_max_used, 0);
	ck_assert(!pool.is_buf_owned);
}
END_TEST

/* testa esvaziar, encher e esvaziar de novo a piscina */
START_TEST(test_pool_alloc_free)
{
	int i;
	struct six* x;

	for (i = 0; i < BUF_LEN; ++i)
	{
		x = pool_alloc(&pool);

		ck_assert_ptr_nonnull(x);
		ck_assert_ptr_eq(x, buf + i);

		ck_assert_int_eq(pool.n_free, BUF_LEN - (i+1));
		ck_assert_int_eq(pool.n_used, i+1);
	}

	for (i = 0; i < BUF_LEN; ++i)
	{
		pool_free(&pool, buf + i);

		ck_assert_int_eq(pool.n_free, i+1);
		ck_assert_int_eq(pool.n_used, BUF_LEN - (i+1));
	}

	for (i = 0; i < BUF_LEN; ++i)
	{
		x = pool_alloc(&pool);

		ck_assert_ptr_nonnull(x);
		ck_assert_ptr_eq(x, buf + BUF_LEN-1 - i);

		ck_assert_int_eq(pool.n_free, BUF_LEN - (i+1));
		ck_assert_int_eq(pool.n_used, i+1);
	}

	ck_assert_int_eq(pool.n_max_used, BUF_LEN);
}
END_TEST

/* teeeeeste */
Suite* pool_suite()
{
	Suite* s;
	TCase* tc_core;

	s = suite_create("pool");
	tc_core = tcase_create("core");
	tcase_add_checked_fixture(tc_core, setup, teardown);

	tcase_add_test(tc_core, test_pool_init);
	tcase_add_test(tc_core, test_pool_alloc_free);
	suite_add_tcase(s, tc_core);

	return s;
}
