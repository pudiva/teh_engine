#ifndef HAS_VEC_H
#define HAS_VEC_H

#define VEC_EPSILON 0.0001

extern const float zero3[3];
extern const float minusz4[4];
extern const float eye4[4][4];

/*
 * copia
 *
 */
void vec4_copy(const float* x, float* y);
void vec3_copy(const float* x, float* y);
void vec2_copy(const float* x, float* y);

/*
 * axpy - a * x + y
 *
 */
void vec4_axpy(float a, const float* x, float* y);
void vec3_axpy(float a, const float* x, float* y);
void vec2_axpy(float a, const float* x, float* y);

/*
 * interpolação linear
 *
 */
void vec4_lerp(float w, const float* x, float* y);
void vec3_lerp(float w, const float* x, float* y);
void vec2_lerp(float w, const float* x, float* y);

/*
 * produto vetorial
 *
 */
void vec3_cross(const float* x, const float* y, float* z);

/*
 * produto interno
 *
 */
float vec4_dot(const float* x, const float* y);
float vec3_dot(const float* x, const float* y);
float vec2_dot(const float* x, const float* y);

/*
 * tamanho
 *
 */
float vec4_len(const float* x);
float vec3_len(const float* x);
float vec2_len(const float* x);

/*
 * normaliza vetor
 *
 */
float vec4_normalize(float* x);
float vec3_normalize(float* x);
float vec2_normalize(float* x);

/*
 * comparação
 *
 */
int vec_cmp(float x, float y);

/*
 * copia matriz
 *
 */
void mat4_copy(const float (*A)[4], float (*B)[4]);

/*
 * matriz * vetor
 *
 */
void mat4_gemv(float a, const float (*A)[4], const float* x, float b, float *y);

/*
 * matriz * matriz
 *
 */
void mat4_gemm(float a, const float (*A)[4], const float (*B)[4], float c, float (*C)[4]);

/*
 * gera matrizes de rotação
 *
 */
void mat4_rotate_x(float a, float (*A)[4]);
void mat4_rotate_y(float a, float (*A)[4]);
void mat4_rotate_z(float a, float (*A)[4]);

/*
 * gera matrizes de translação
 *
 */
void mat4_translate(const float* x, float (*A)[4]);

/*
 * transpõe matriz
 *
 */
void mat4_transpose(const float (*A)[4], float (*B)[4]);

/*
 * inverte matrizes compostas por rotações e translações
 *
 */
void mat4_magic_inv(const float (*A)[4], float (*B)[4]);

#if DEBUG
#include <stdio.h>
#include <string.h>
char* vec4_2_str(const float *x);
char* mat4_2_str(const float (*A)[4]);
#endif

#endif
