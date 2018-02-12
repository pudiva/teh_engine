#include <math.h>
#include "vec.h"

const float zero3[3] = {0, 0, 0};

void vec4_copy(const float* restrict x, float* restrict y)
{
	y[0] = x[0];
	y[1] = x[1];
	y[2] = x[2];
	y[3] = x[3];
}

void vec3_copy(const float* restrict x, float* restrict y)
{
	y[0] = x[0];
	y[1] = x[1];
	y[2] = x[2];
}

void vec3_axpy(float a, const float* restrict x, float* restrict y)
{
	y[0] += a*x[0];
	y[1] += a*x[1];
	y[2] += a*x[2];
}

void vec3_lerp(float w, const float* x, float* y)
{
	y[0] = x[0]*(1-w) + y[0]*w;
	y[1] = x[1]*(1-w) + y[1]*w;
	y[2] = x[2]*(1-w) + y[2]*w;
}

void vec3_cross(const float* restrict x, const float* restrict y, float* restrict z)
{
	z[0] = x[1]*y[2] - x[2]*y[1];
	z[1] = x[2]*y[0] - x[0]*y[2];
	z[2] = x[0]*y[1] - x[1]*y[0];
}

float vec3_dot(const float* restrict x, const float* restrict y)
{
	return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
}

float vec3_sqrlen(const float* x)
{
	return x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
}

float vec3_len(const float* x)
{
	return sqrt(vec3_sqrlen(x));
}

float vec3_normalize(float* x)
{
	float l;

	l = vec3_len(x);
	x[0] /= l;
	x[1] /= l;
	x[2] /= l;

	return l;
}

void vec2_copy(const float* restrict x, float* restrict y)
{
	y[0] = x[0];
	y[1] = x[1];
}

void vec2_axpy(float a, const float* restrict x, float* restrict y)
{
	y[0] += a * x[0];
	y[1] += a * x[1];
}
void vec2_lerp(float w, const float* restrict x, float* restrict y)
{
	y[0] = x[0]*(1-w) + y[0]*w;
	y[1] = x[1]*(1-w) + y[1]*w;
}

int vec_cmp(float x, float y)
{
	float z;
	z = x - y;

	if (fabs(z) < VEC_EPSILON)
		return 0;

	else if (z < 0)
		return -1;

	else
		return 1;
}
