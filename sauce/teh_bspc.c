#include <assert.h>
#include <stdio.h>
#include <gvc.h>

#include "teh_model.h"
#include "bspc.h"
#include "vec.h"
#include "assets.h"

struct node* root;

#ifdef GRAPH
graph_t *g;
static Agnode_t* graph_node(struct node* n)
{
	Agnode_t* agn, * agn_b, * agn_f;
	char name[16];

	if (!n)
		return NULL;

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
#endif

int main(int argc, char *argv[])
{
	struct teh_model* model;
	struct teh_bsp* bsp;

	assert (argc == 3);
	model = teh_model_get(argv[1]);

	printf("compiling model with %d triangles and %d frames\n",
			model->n_tris, model->n_frames);

	assert (0 < model->n_tris);
	assert (0 < model->n_frames);

	bsp = teh_bspc(model);
	assert (bsp);

	assert (0 < bsp->model.n_tris);
	assert (0 < bsp->n_nodes);

	printf("bsp has %d nodes and %d triangles\n",
			bsp->n_nodes, bsp->model.n_tris);

	teh_bsp_write_file(bsp, argv[2]);
	printf("written to %s\n", argv[2]);

#ifdef GRAPH
	//graph();
#endif

	return 0;
}
