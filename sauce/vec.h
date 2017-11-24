#define VEC_EPSILON 0.0001

extern const float zero3[3];

void vec4_copy(const float* restrict x, float* restrict y);
void vec3_copy(const float* restrict x, float* restrict y);
void vec3_axpy(float a, const float* restrict x, float* restrict y);
void vec3_lerp(float w, const float* x, float* y);
void vec3_cross(const float* restrict x, const float* restrict y, float* restrict z);
float vec3_dot(const float* restrict x, const float* restrict y);
float vec3_sqrlen(const float* x);
float vec3_len(const float* x);
float vec3_normalize(float* x);
void vec2_copy(const float* restrict x, float* restrict y);
void vec2_axpy(float a, const float* restrict x, float* restrict y);
void vec2_lerp(float w, const float* restrict x, float* restrict y);
int vec_cmp(float x, float y);
