#include "vec.h"

#define assert_vec2_eq(a, b) \
	ck_assert_msg( \
		!vec_cmp((a)[0], (b)[0]) && \
		!vec_cmp((a)[1], (b)[1]) && \
		!vec_cmp((a)[2], (b)[2]), \
		"vectors " #a" and "#b" are different: {%f, %f} and {%f, %f}\n", \
		a[0], a[1], b[0], b[1] \
		)

#define assert_vec3_eq(a, b) \
	ck_assert_msg( \
		!vec_cmp((a)[0], (b)[0]) && \
		!vec_cmp((a)[1], (b)[1]) && \
		!vec_cmp((a)[2], (b)[2]), \
		"vectors " #a" and "#b" are different: {%f, %f, %f} and {%f, %f, %f}\n", \
		a[0], a[1], a[2], b[0], b[1], b[2] \
		)

#define assert_vec4_eq(a, b) \
	ck_assert_msg( \
		!vec_cmp((a)[0], (b)[0]) && \
		!vec_cmp((a)[1], (b)[1]) && \
		!vec_cmp((a)[2], (b)[2]) && \
		!vec_cmp((a)[3], (b)[3]), \
		"vectors " #a" and "#b" are different: {%f, %f, %f, %f} and {%f, %f, %f, %f}\n", \
		a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3] \
		)
