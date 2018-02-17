#include "r_beh.h"
#include "r_teh.h"
#include "vec.h"

static inline void draw_node(struct beh* bsp, struct beh_node* n, const float* eye)
{
	float h;

	if (!n) return;
	h = vec3_dot(n->plane, eye);

	if (n->is_leaf)
	{
		/* folhas líquidass */
		if (!n->is_solid)
			r_teh(&bsp->model, 0, 0, 0, n->off, n->size);
	}

	else
	{
		/* tamo atras */
		if (h < n->plane[3])
		{
			draw_node(bsp, n->front, eye);
			draw_node(bsp, n->back, eye);
		}

		/* tamo na frenty */
		else
		{
			draw_node(bsp, n->back, eye);
			draw_node(bsp, n->front, eye);
		}
	}
}

void r_beh_from_eye(struct beh* bsp, const float* eye)
{
	glDisable(GL_DEPTH_TEST);
	draw_node(bsp, bsp->nodes, eye);
	glEnable(GL_DEPTH_TEST);
}
