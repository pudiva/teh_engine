#include <assert.h>
#include "beh.h"

/*
 * lê beh de uma fonte
 *
 */
static void inline scan_beh_node(int i, struct beh_node* n, FILE* fp)
{
	int r, back, front;
	signed char is_leaf, is_solid; /* FIXME */

	r = fscanf(fp, "%hhd %hhd %f %f %f %f %d %d %d %d\n",
			&is_leaf, &is_solid,
			n[i].plane+0, n[i].plane+1, n[i].plane+2, n[i].plane+3,
			n[i].i+0, n[i].i+1,
			&back, &front);

	assert (r == 10);

	n[i].type = is_solid ? SOLID_LEAF : (is_leaf ? LEAF : NON_LEAF);
	n[i].kids[0] = -1 < back ? n + back : NULL;
	n[i].kids[1] = -1 < front ? n + front : NULL;
}

void beh_read(struct beh* bsp, FILE* fp)
{
	int i, r;

	memset(bsp, 0, sizeof (struct beh));

	fscanf(fp, "dota? =op\n");
	fscanf(fp, "teh_model is\n");
	teh_read(&bsp->model, fp);

	r = fscanf(fp, "tehre are %d nodes. ehre tehy are\n", &bsp->n_nodes);

	assert (r == 1);

	bsp->nodes = calloc(bsp->n_nodes, sizeof (struct beh_node));
	assert (bsp->nodes);

	for (i = 0; i < bsp->n_nodes; ++i)
		scan_beh_node(i, bsp->nodes, fp);
}

/*
 * escreve beh em uma fonte
 *
 */
static void inline print_beh_node(int i, struct beh_node* n, FILE* fp)
{
	long back, front;

	back = n[i].kids[0] ? (n[i].kids[0] - n) : -1;
	front = n[i].kids[1] ? (n[i].kids[1] - n) : -1;

	/* FIXME: is_leaf, is_solid */
	fprintf(fp, "%hhd %hhd %f %f %f %f %d %d %ld %ld\n",
			n[i].type & LEAF, (n[i].type & SOLID_LEAF) == SOLID_LEAF,
			n[i].plane[0], n[i].plane[1], n[i].plane[2], n[i].plane[3],
			n[i].i[0], n[i].i[1],
			back, front);
}

void beh_write(struct beh* bsp, FILE* fp)
{
	int i;

	fprintf(fp, "dota? =op\n");
	fprintf(fp, "teh_model is\n");
	teh_write(&bsp->model, fp);

	fprintf(fp, "tehre are %d nodes. ehre tehy are\n", bsp->n_nodes);
	for (i = 0; i < bsp->n_nodes; ++i)
		print_beh_node(i, bsp->nodes, fp);
}

/*
 * lê beh de um arquivo
 *
 */
void beh_read_file(struct beh* bsp, const char* path)
{
	FILE* fp;
	fp = fopen(path, "r");
	assert (fp);
	beh_read(bsp, fp);
	fclose(fp);
}

/*
 * escreve beh em um arquivo
 *
 */
void beh_write_file(struct beh* bsp, const char* path)
{
	FILE* fp;
	fp = fopen(path, "w");
	assert (fp);
	beh_write(bsp, fp);
	fclose(fp);
}
