#include <string.h>
#include <math.h>
#include "vec.h"

const float zero3[3] = {0, 0, 0};
const float minusz4[4] = { 0,  0, -1,  0};
const float eye4[4][4] =
{
	{1, 0, 0, 0},
	{0, 1, 0, 0},
	{0, 0, 1, 0},
	{0, 0, 0, 1}
};

/*
 * copia
 *
 */
void vec4_copy(const float* x, float* y)
{
	y[0] = x[0];
	y[1] = x[1];
	y[2] = x[2];
	y[3] = x[3];
}

void vec3_copy(const float* x, float* y)
{
	y[0] = x[0];
	y[1] = x[1];
	y[2] = x[2];
}

void vec2_copy(const float* x, float* y)
{
	y[0] = x[0];
	y[1] = x[1];
}

/*
 * axpy - a * x + y
 *
 */
void vec4_axpy(float a, const float* x, float* y)
{
	y[0] += a*x[0];
	y[1] += a*x[1];
	y[2] += a*x[2];
	y[3] += a*x[3];
}

void vec3_axpy(float a, const float* x, float* y)
{
	y[0] += a*x[0];
	y[1] += a*x[1];
	y[2] += a*x[2];
}

void vec2_axpy(float a, const float* x, float* y)
{
	y[0] += a*x[0];
	y[1] += a*x[1];
}

/*
 * interpolação linear
 *
 */
void vec4_lerp(float w, const float* x, float* y)
{
	y[0] = (1-w)*x[0] + w*y[0];
	y[1] = (1-w)*x[1] + w*y[1];
	y[2] = (1-w)*x[2] + w*y[2];
	y[3] = (1-w)*x[3] + w*y[3];
}

void vec3_lerp(float w, const float* x, float* y)
{
	y[0] = (1-w)*x[0] + w*y[0];
	y[1] = (1-w)*x[1] + w*y[1];
	y[2] = (1-w)*x[2] + w*y[2];
}

void vec2_lerp(float w, const float* x, float* y)
{
	y[0] = (1-w)*x[0] + w*y[0];
	y[1] = (1-w)*x[1] + w*y[1];
}

/*
 * produto vetorial
 *
 */
void vec3_cross(const float* x, const float* y, float* z)
{
	z[0] = x[1]*y[2] - x[2]*y[1];
	z[1] = x[2]*y[0] - x[0]*y[2];
	z[2] = x[0]*y[1] - x[1]*y[0];
}

/*
 * produto interno
 *
 */
float vec4_dot(const float* x, const float* y)
{
	return x[0]*y[0] + x[1]*y[1] + x[2]*y[2] + x[3]*y[3];
}

float vec3_dot(const float* x, const float* y)
{
	return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
}

float vec2_dot(const float* x, const float* y)
{
	return x[0]*y[0] + x[1]*y[1];
}

/*
 * tamanho
 *
 */
float vec4_len(const float* x)
{
	return sqrt(vec4_dot(x, x));
}

float vec3_len(const float* x)
{
	return sqrt(vec3_dot(x, x));
}

float vec2_len(const float* x)
{
	return sqrt(vec2_dot(x, x));
}

/*
 * normaliza vetor
 *
 */
float vec4_normalize(float* x)
{
	float l;

	l = vec4_len(x);
	if (l)
	{
		x[0] /= l;
		x[1] /= l;
		x[2] /= l;
		x[3] /= l;
	}

	return l;
}

float vec3_normalize(float* x)
{
	float l;

	l = vec3_len(x);
	if (l)
	{
		x[0] /= l;
		x[1] /= l;
		x[2] /= l;
	}

	return l;
}

float vec2_normalize(float* x)
{
	float l;

	l = vec2_len(x);
	if (l)
	{
		x[0] /= l;
		x[1] /= l;
	}

	return l;
}

/*
 * comparação
 *
 */
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

/*
 * copia matriz
 *
 */
void mat4_copy(const float (*A)[4], float (*B)[4])
{
	memcpy(B, A, sizeof (float[4][4]));
}

/*
 * matriz * vetor
 *
 */
void mat4_gemv(float a, const float (*A)[4], const float* x, float b, float *y)
{
	y[0] = a * ((A[0][0] * x[0]) + (A[1][0] * x[1]) + (A[2][0] * x[2]) + (A[3][0] * x[3])) + b * y[0];
	y[1] = a * ((A[0][1] * x[0]) + (A[1][1] * x[1]) + (A[2][1] * x[2]) + (A[3][1] * x[3])) + b * y[1];
	y[2] = a * ((A[0][2] * x[0]) + (A[1][2] * x[1]) + (A[2][2] * x[2]) + (A[3][2] * x[3])) + b * y[2];
	y[3] = a * ((A[0][3] * x[0]) + (A[1][3] * x[1]) + (A[2][3] * x[2]) + (A[3][3] * x[3])) + b * y[3];
}

/*
 * matriz * matriz
 *
 */
void mat4_gemm(float a, const float (*A)[4], const float (*B)[4], float c, float (*C)[4])
{
	mat4_gemv(a, A, B[0], c, C[0]);
	mat4_gemv(a, A, B[1], c, C[1]);
	mat4_gemv(a, A, B[2], c, C[2]);
	mat4_gemv(a, A, B[3], c, C[3]);
}

/*
 * gera matrizes de rotação
 *
 */
void mat4_rotate_x(float a, float (*A)[4])
{
	float s, c;

	s = sin(a);
	c = cos(a);

	A[0][0] =  1; A[1][0] =  0; A[2][0] =  0; A[3][0] =  0;
	A[0][1] =  0; A[1][1] =  c; A[2][1] = -s; A[3][1] =  0;
	A[0][2] =  0; A[1][2] =  s; A[2][2] =  c; A[3][2] =  0;
	A[0][3] =  0; A[1][3] =  0; A[2][3] =  0; A[3][3] =  1;
}

void mat4_rotate_y(float a, float (*A)[4])
{
	float s, c;

	s = sin(a);
	c = cos(a);

	A[0][0] =  c; A[1][0] =  0; A[2][0] =  s; A[3][0] =  0;
	A[0][1] =  0; A[1][1] =  1; A[2][1] =  0; A[3][1] =  0;
	A[0][2] = -s; A[1][2] =  0; A[2][2] =  c; A[3][2] =  0;
	A[0][3] =  0; A[1][3] =  0; A[2][3] =  0; A[3][3] =  1;
}

void mat4_rotate_z(float a, float (*A)[4])
{
	float s, c;

	s = sin(a);
	c = cos(a);

	A[0][0] =  c; A[1][0] = -s; A[2][0] =  0; A[3][0] =  0;
	A[0][1] =  s; A[1][1] =  c; A[2][1] =  0; A[3][1] =  0;
	A[0][2] =  0; A[1][2] =  0; A[2][2] =  1; A[3][2] =  0;
	A[0][3] =  0; A[1][3] =  0; A[2][3] =  0; A[3][3] =  1;
}

/*
 * gera matrizes de translação
 *
 */
void mat4_translate(const float* x, float (*A)[4])
{
	A[0][0] =  1   ; A[1][0] =  0   ; A[2][0] =  0   ; A[3][0] =  x[0];
	A[0][1] =  0   ; A[1][1] =  1   ; A[2][1] =  0   ; A[3][1] =  x[1];
	A[0][2] =  0   ; A[1][2] =  0   ; A[2][2] =  1   ; A[3][2] =  x[2];
	A[0][3] =  0   ; A[1][3] =  0   ; A[2][3] =  0   ; A[3][3] =  1   ;
}

/*
 * transpõe matriz
 *
 */
void mat4_transpose(const float (*A)[4], float (*B)[4])
{
	B[0][0] = A[0][0]; B[0][1] = A[1][0]; B[0][2] = A[2][0]; B[0][3] = A[3][0];
	B[1][0] = A[0][1]; B[1][1] = A[1][1]; B[1][2] = A[2][1]; B[1][3] = A[3][1];
	B[2][0] = A[0][2]; B[2][1] = A[1][2]; B[2][2] = A[2][2]; B[2][3] = A[3][2];
	B[3][0] = A[0][3]; B[3][1] = A[1][3]; B[3][2] = A[2][3]; B[3][3] = A[3][3];
}

/*
 * inverte matrizes compostas por rotações e translações
 *
 */
void mat4_magic_inv(const float (*A)[4], float (*B)[4])
{
	mat4_transpose(A, B);
	B[3][0] = -(B[0][3] * B[0][0] + B[1][3] * B[1][0] + B[2][3] * B[2][0]);
	B[3][1] = -(B[0][3] * B[0][1] + B[1][3] * B[1][1] + B[2][3] * B[2][1]);
	B[3][2] = -(B[0][3] * B[0][2] + B[1][3] * B[1][2] + B[2][3] * B[2][2]);
	B[0][3] = 0;
	B[1][3] = 0;
	B[2][3] = 0;
}

#if DEBUG
#include <stdio.h>
#include <string.h>
char* vec4_2_str(const float *x)
{
	static char str[128] = {0};
	sprintf(str, "{% f,\t% f,\t% f,\t% f}", x[0], x[1], x[2], x[3]);
	return str;
}

char* mat4_2_str(const float (*A)[4])
{
	static char str[1024] = {0};
	str[0] = 0;
	strcat(str, "{\n\t");
	strcat(str, vec4_2_str(A[0]));
	strcat(str, "\n\t");
	strcat(str, vec4_2_str(A[1]));
	strcat(str, "\n\t");
	strcat(str, vec4_2_str(A[2]));
	strcat(str, "\n\t");
	strcat(str, vec4_2_str(A[3]));
	strcat(str, "\n}");
	return str;
}
#endif
