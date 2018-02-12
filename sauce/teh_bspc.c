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
	for (i = 0; i < in.n_tris; ++i)
	{
		(*px) = tri_alloc();

		for (j = 0; j < 3; ++j)
		{
			vec3_copy(in.tris[i][j], (*px)->v[j]);
			vec2_copy(in.texcoords[i][j], (*px)->tc[j]);
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

	out.n_tris = i;
	out.n_frames = 1;
	out.texture = in.texture;

	printf("out.n_tris = %d\n", out.n_tris);
	printf("in.n_tris = %d\n", in.n_tris);

	out.tris = calloc(i, sizeof (float[3][3]));
	out.texcoords = calloc(i, sizeof (float[3][2]));

	for (i = 0, cur = list; cur; ++i, cur = cur->next)
	{
		for (j = 0; j < 3; ++j)
		{
			vec3_copy(cur->v[j], out.tris[i][j]);
			vec2_copy(cur->tc[j], out.texcoords[i][j]);
		}
	}
}

static void assert_in_eq_out()
{
	int i, j;

	assert (out.n_tris == in.n_tris);

	for (i = 0; i < out.n_tris; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			assert (in.tris[i][j][0] == out.tris[i][j][0]);
			assert (in.tris[i][j][1] == out.tris[i][j][1]);
			assert (in.tris[i][j][2] == out.tris[i][j][2]);

			assert (in.texcoords[i][j][0] == out.texcoords[i][j][0]);
			assert (in.texcoords[i][j][1] == out.texcoords[i][j][1]);
		}
	}
}

int main(int argc, char *argv[])
{
	assert (argc == 2);
	teh_model_read_file(&in, argv[1]);

	fprintf(stderr, "compiling model with %d triangles and %d frames\n",
			in.n_tris, in.n_frames);

	assert (0 < in.n_tris);
	assert (0 < in.n_frames);

	to_tris();
	assert (list);

	memset (&out, 0, sizeof (out));
	to_model();

	assert_in_eq_out();

	root = bspc(list);
	fprintf(stderr, "bsp has %d nodes\n", node_pool_c);
	assert (root);

	graph();

	teh_model_free(&in);
	return 0;
}
