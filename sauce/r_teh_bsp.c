#include "r_teh_bsp.h"
#include "r_teh_model.h"
#include "vec.h"

static inline void draw_node(struct teh_bsp* bsp, struct teh_bsp_node* n, const float* eye)
{
	float h;

	if (!n) return;
	h = vec3_dot(n->plane, eye);

	draw_node(bsp, (h < 0 ? n->front : n->back), eye);
	r_teh_model(&bsp->model, 0, 0, 0, n->off, n->size);
	draw_node(bsp, (h < 0 ? n->back : n->front), eye);
}

void r_teh_bsp_from_eye(struct teh_bsp* bsp, const float* eye)
{
	draw_node(bsp, bsp->nodes, eye);
}
