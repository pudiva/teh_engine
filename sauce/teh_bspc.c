#include <assert.h>

#include "teh_model.h"
#include "bspc.h"
#include "vec.h"
#include <gvc.h>

struct teh_model in, out;
struct node* root;

struct tri* list;

graph_t *g;
static Agnode_t* graph_node(struct node* n)
{
	Agnode_t* agn, * agn_b, * agn_f;
	char name[16];

	if (!n)
	{
		return NULL;
	}

	sprintf(name, "%ld: %c",
			n - node_pool,
			n->is_leaf ? (n->is_solid ? 'S' : 'L') : 'N');

	agn = agnode(g, name, 1);
	agn_b = graph_node(n->back);
	agn_f = graph_node(n->front);

	if (agn_b)
		agedge(g, agn, agn_b, 0, 1);

	if (agn_f)
		agedge(g, agn, agn_f, 0, 1);

	return agn;
}

static void graph()
{
	GVC_t *gvc;
	FILE* fp;

	fp = fopen("le_pdf.ps", "w");
	gvc = gvContext();
	g = agopen("g", Agdirected, 0);
	agattr(g, AGRAPH, "ordering", "out");

	graph_node(root);

	gvLayout(gvc, g, "dot");
	gvRender(gvc, g, "ps2", fp);
	gvFreeLayout(gvc, g);

	agclose(g);
	gvFreeContext(gvc);
	fclose(fp);
}

static void to_tris()
{
	int i, j;
	struct tri** px;
	float e1[3], e2[3];

	px = &list;
	list = NULL;
	for (i = 0; i < in.triangle_c; ++i)
	{
		(*px) = tri_alloc();

		for (j = 0; j < 3; ++j)
		{
			vec3_copy(in.vertex_v[3*i+j], (*px)->v[j]);
			vec2_copy(in.tex_coord_v[3*i+j], (*px)->tc[j]);
		}

		vec3_copy((*px)->v[1], e1);
		vec3_axpy(-1, (*px)->v[0], e1);

		vec3_copy((*px)->v[2], e2);
		vec3_axpy(-1, (*px)->v[0], e2);

		vec3_cross(e1, e2, (*px)->p);
		vec3_normalize((*px)->p);
		(*px)->p[3] = vec3_dot((*px)->v[0], (*px)->p);

		assert ((*px) != (*px)->next);
		px = &(*px)->next;

		assert ((*px) == NULL);
	}
}

static void to_model()
{
	struct tri* cur;
	int i, j;

	for (i = 0, cur = list; cur; ++i, cur = cur->next);

	out.triangle_c = i;
	out.frame_c = 1;
	out.texture = in.texture;

	printf("out.triangle_c = %d\n", out.triangle_c);
	printf("in.triangle_c = %d\n", in.triangle_c);

	out.vertex_v = calloc(3*i, sizeof (float[3]));
	out.tex_coord_v = calloc(3*i, sizeof (float[2]));

	for (i = 0, cur = list; cur; ++i, cur = cur->next)
	{
		for (j = 0; j < 3; ++j)
		{
			vec3_copy(cur->v[j], out.vertex_v[3*i+j]);
			vec2_copy(cur->tc[j], out.tex_coord_v[3*i+j]);
		}
	}
}

static void assert_in_eq_out()
{
	int i, j;

	assert (out.triangle_c == in.triangle_c);

	for (i = 0; i < out.triangle_c; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			assert (in.vertex_v[3*i+j][0] == out.vertex_v[3*i+j][0]);
			assert (in.vertex_v[3*i+j][1] == out.vertex_v[3*i+j][1]);
			assert (in.vertex_v[3*i+j][2] == out.vertex_v[3*i+j][2]);

			assert (in.tex_coord_v[3*i+j][0] == out.tex_coord_v[3*i+j][0]);
			assert (in.tex_coord_v[3*i+j][1] == out.tex_coord_v[3*i+j][1]);
		}
	}
}

int main(int argc, char *argv[])
{
	assert (argc == 2);
	read_teh_model_vertices(&in, argv[1]);

	fprintf(stderr, "compiling model with %d triangles and %d frames\n", in.triangle_c, in.frame_c);
	assert (0 < in.triangle_c);
	assert (0 < in.frame_c);

	to_tris();
	assert (list);

	memset (&out, 0, sizeof (out));
	to_model();

	assert_in_eq_out();

	root = bspc(list);
	fprintf(stderr, "bsp has %d nodes\n", node_pool_c);
	assert (root);

	graph();

	free_teh_model(&in);
	return 0;
}
