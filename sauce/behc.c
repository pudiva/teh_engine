#include <assert.h>
#include <stdio.h>
#include <gvc.h>

#include "teh.h"
#include "bspc.h"
#include "vec.h"
#include "assets.h"
#include "poly_pool.h"

struct beh_node* root;

#define GRAPH

#ifdef GRAPH
graph_t *g;
static Agnode_t* graph_node(struct beh_node* n)
{
	Agnode_t* agn, * agn_b, * agn_f;
	char name[32];

	if (!n)
		return NULL;

	switch (n->type)
	{
	case LEAF:
		sprintf(name, "%ld: L (%d)", n - node_pool, n->i[1]);
		break;
	case SOLID_LEAF:
		sprintf(name, "%ld: S", n - node_pool);
		break;
	default:
		sprintf(name, "%ld: N", n - node_pool);
	}

	agn = agnode(g, name, 1);
	agn_b = graph_node(n->kids[0]);
	agn_f = graph_node(n->kids[1]);

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
#endif

int main(int argc, char *argv[])
{
	struct teh* model;
	struct beh* bsp;

	assert (argc == 3);
	model = teh_get(argv[1]);

	printf("compiling model with %d triangles and %d frames\n",
			model->n_tris, model->n_frames);

	assert (0 < model->n_tris);
	assert (0 < model->n_frames);

	poly_pool_init();
	bsp = behc(model);
	assert (bsp);

	assert (0 < bsp->model.n_tris);
	assert (0 < bsp->n_nodes);

	printf("bsp has %d nodes and %d triangles\n",
			bsp->n_nodes, bsp->model.n_tris);

	beh_write_file(bsp, argv[2]);
	printf("written to %s\n", argv[2]);

#ifdef GRAPH
	root = node_pool;
	graph();
#endif

	return 0;
}
