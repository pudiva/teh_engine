#include "r_teh_bsp.h"
#include "r_teh_model.h"
#include "vec.h"

#include <stdio.h>

static inline void draw_node(struct teh_bsp* bsp, struct teh_bsp_node* n, const float* eye)
{
	float h;

	if (!n) return;
	h = vec3_dot(n->plane, eye);

	if (n->is_leaf)
	{
		/* folhas líquidass */
		if (!n->is_solid)
			r_teh_model(&bsp->model, 0, 0, 0, n->off, n->size);
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

void r_teh_bsp_from_eye(struct teh_bsp* bsp, const float* eye)
{
	draw_node(bsp, bsp->nodes, eye);
}
