#include "r_beh.h"
#include "r_teh.h"
#include "vec.h"

static inline void draw_node(struct beh* bsp, struct beh_node* n, const float* eye)
{
	float h;

	if (!n) return;
	h = vec3_dot(n->plane, eye);

	if ((n->type & LEAF) == LEAF)
	{
		/* folhas líquidass */
		r_teh(&bsp->model, 0, 0, 0, n->i[0], n->i[1]);
	}

	else
	{
		/* tamo atras */
		if (h < n->plane[3])
		{
			draw_node(bsp, n->kids[1], eye);
			draw_node(bsp, n->kids[0], eye);
		}

		/* tamo na frenty */
		else
		{
			draw_node(bsp, n->kids[0], eye);
			draw_node(bsp, n->kids[1], eye);
		}
	}
}

void r_beh_from_eye(struct beh* bsp, const float* eye)
{
	draw_node(bsp, bsp->nodes, eye);
}
