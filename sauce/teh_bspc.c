#include <assert.h>
#include <stdio.h>
#include <gvc.h>

#include "teh_model.h"
#include "bspc.h"
#include "vec.h"
#include "assets.h"

struct node* root;

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

int main(int argc, char *argv[])
{
	struct tri* list;
	struct teh_model* in;

	assert (argc == 2);
	in = teh_model_get(argv[1]);

	fprintf(stderr, "compiling model with %d triangles and %d frames\n",
			in->n_tris, in->n_frames);

	assert (0 < in->n_tris);
	assert (0 < in->n_frames);

	list = tri_from_teh_model(in);
	assert (list);

	root = bspc(list);
	assert (root);

	fprintf(stderr, "bsp has %d nodes\n", node_pool_c);
	graph();
	return 0;
}
