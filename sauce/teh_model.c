#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "renderer.h"
#include "teh_model.h"

#include <SDL_image.h>

/*
 * lê vetores
 *
 */
static void inline scan_vec3(float* x, FILE* fp)
{
	int r;
	r = fscanf(fp, "%f %f %f\n", x, x+1, x+2);
	assert (r == 3);
}

static void inline scan_vec2(float* x, FILE* fp)
{
	int r;
	r = fscanf(fp, "%f %f\n", x, x+1);
	assert (r == 2);
}

/*
 * escreve vetores
 *
 */
static void print_vec3(float* x, FILE* fp)
{
	fprintf(fp, "%f %f %f\n", x[0], x[1], x[2]);
}

static void print_vec2(float* x, FILE* fp)
{
	fprintf(fp, "%f %f\n", x[0], x[1]);
}

/*
 * lê teh_model de uma fonte
 *
 */
void teh_model_read(struct teh_model* x, FILE* fp)
{
	int i, j, k, r;

	memset(x, 0, sizeof (*x));

	fscanf(fp, "dota? =op\n");
	r = fscanf(fp, "teh model has %d triangles and %d frames.\n",
			&x->n_tris, &x->n_frames);

	assert (r == 2);

	x->texcoords = calloc(x->n_tris, sizeof (float[3][2]));
	x->tris = calloc(x->n_frames * x->n_tris, sizeof (float[3][3]));

	fscanf(fp, "teh texcoords are\n");
	for (i = 0; i < x->n_tris; ++i)
		for (j = 0; j < 3; ++j)
			scan_vec2(x->texcoords[i][j], fp);

	fscanf(fp, "and teh vertices are\n");
	for (k = 0; k < x->n_frames; ++k)
		for (i = 0; i < x->n_tris; ++i)
			for (j = 0; j < 3; ++j)
				scan_vec3(x->tris[k * x->n_tris + i][j], fp);
}

/*
 * escreve teh_model em uma fonte
 *
 */
void teh_model_write(struct teh_model* x, FILE* fp)
{
	int i, j, k;

	fprintf(fp, "dota? =op\n");
	fprintf(fp, "teh model has %d triangles and %d frames.\n",
			x->n_tris, x->n_frames);

	fprintf(fp, "teh texcoords are\n");
	for (i = 0; i < x->n_tris; ++i)
		for (j = 0; j < 3; ++j)
			print_vec2(x->texcoords[i][j], fp);

	fprintf(fp, "and teh vertices are\n");
	for (k = 0; k < x->n_frames; ++k)
		for (i = 0; i < x->n_tris; ++i)
			for (j = 0; j < 3; ++j)
				print_vec3(x->tris[k * x->n_tris + i][j], fp);
}

/*
 * lê teh_model de um arquivo
 *
 */
void teh_model_read_file(struct teh_model* x, const char* path)
{
	FILE* fp;
	fp = fopen(path, "r");
	assert (fp);
	teh_model_read(x, fp);
	fclose(fp);
}

/*
 * escreve teh_model em um arquivo
 *
 */
void teh_model_write_file(struct teh_model* x, const char* path)
{
	FILE* fp;
	fp = fopen(path, "w");
	assert (fp);
	teh_model_write(x, fp);
	fclose(fp);
}

/*
 * liberdady
 *
 */
void teh_model_free(struct teh_model* x)
{
	free(x->tris);
	free(x->texcoords);
}
