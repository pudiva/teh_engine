#include <assert.h>
#include "teh_bsp.h"

/*
 * lê teh_bsp de uma fonte
 *
 */
static void inline scan_teh_bsp_node(int i, struct teh_bsp_node* n, FILE* fp)
{
	int r, back, front;
	r = fscanf(fp, "%hhd %hhd %f %f %f %f %d %d %d %d\n",
			(char*) &n[i].is_leaf, (char*) &n[i].is_solid,
			n[i].plane, n[i].plane+1, n[i].plane+2, n[i].plane+3,
			&n[i].off, &n[i].size,
			&back, &front);

	assert (r == 10);

	n->back = -1 < back ? n + back : NULL;
	n->front = -1 < front ? n + front : NULL;
}

void teh_bsp_read(struct teh_bsp* bsp, FILE* fp)
{
	int i, r;

	memset(bsp, 0, sizeof (struct teh_bsp));

	fscanf(fp, "dota? =op\n");
	fscanf(fp, "teh_model is\n");
	teh_model_read(&bsp->model, fp);

	r = fscanf(fp, "tehre are %d nodes. ehre tehy are\n", &bsp->n_nodes);

	assert (r == 1);

	bsp->nodes = calloc(bsp->n_nodes, sizeof (struct teh_bsp_node));
	assert (bsp->nodes);

	for (i = 0; i < bsp->n_nodes; ++i)
		scan_teh_bsp_node(i, bsp->nodes, fp);
}

/*
 * escreve teh_bsp em uma fonte
 *
 */
static void inline print_teh_bsp_node(int i, struct teh_bsp_node* n, FILE* fp)
{
	long back, front;

	back = n[i].back ? (n[i].back - n) : -1;
	front = n[i].front ? (n[i].front - n) : -1;

	fprintf(fp, "%hhd %hhd %f %f %f %f %d %d %ld %ld\n",
			n[i].is_leaf, n[i].is_solid,
			n[i].plane[0], n[i].plane[1], n[i].plane[2], n[i].plane[3],
			n[i].off, n[i].size,
			back, front);
}

void teh_bsp_write(struct teh_bsp* bsp, FILE* fp)
{
	int i;

	fprintf(fp, "dota? =op\n");
	fprintf(fp, "teh_model is\n");
	teh_model_write(&bsp->model, fp);

	fprintf(fp, "tehre are %d nodes. ehre tehy are\n", bsp->n_nodes);
	for (i = 0; i < bsp->n_nodes; ++i)
		print_teh_bsp_node(i, bsp->nodes, fp);
}

/*
 * lê teh_bsp de um arquivo
 *
 */
void teh_bsp_read_file(struct teh_bsp* bsp, const char* path)
{
	FILE* fp;
	fp = fopen(path, "r");
	assert (fp);
	teh_bsp_read(bsp, fp);
	fclose(fp);
}

/*
 * escreve teh_bsp em um arquivo
 *
 */
void teh_bsp_write_file(struct teh_bsp* bsp, const char* path)
{
	FILE* fp;
	fp = fopen(path, "w");
	assert (fp);
	teh_bsp_write(bsp, fp);
	fclose(fp);
}
