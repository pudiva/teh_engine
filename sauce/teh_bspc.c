#include <assert.h>

#include "teh_model.h"
#include "bspc.h"
#include "vec.h"

struct teh_model in;
struct teh_model out;
struct node* node;

struct tri* list;

void make_tris()
{
	int i, j;
	struct tri* x;
	float e1[3], e2[3];

	for (i = 0; i < in.triangle_c; ++i)
	{
		x = tri_alloc();

		for (j = 0; j < 3; ++j)
		{
			vec3_copy(in.vertex_v[3*i+j], x->v[j]);
			vec2_copy(in.tex_coord_v[3*i+j], x->tc[j]);
		}

		vec3_copy(x->v[1], e1);
		vec3_axpy(-1, x->v[0], e1);

		vec3_copy(x->v[2], e2);
		vec3_axpy(-1, x->v[0], e2);

		vec3_cross(x->p, e1, e2);
		vec3_normalize(x->p);
	}
}

int main(int argc, char *argv[])
{
	assert (argc == 2);
	read_teh_model_vertices(&in, argv[1]);
	assert (0 < in.triangle_c);
	assert (0 < in.frame_c);

	node = bspc(list);

	assert (node);

	free_teh_model(&in);
	return 0;
}
