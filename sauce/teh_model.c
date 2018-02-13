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

/*
 * carrega VBO
 *
 */
void r_teh_model_load_vbo(struct teh_model* x)
{
	int vbo_size;

	glGenBuffers(1, &x->vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, x->vbo_id);

	x->vbo_texcoords_off = 0;
	x->vbo_tris_off = x->n_tris * sizeof(float[3][2]);
	x->vbo_frame_size = x->n_tris * sizeof (float[3][3]);
	vbo_size = x->n_frames * x->n_tris * sizeof (float[3][5]);
		
	glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, x->vbo_texcoords_off,
			x->vbo_tris_off, x->texcoords);
	glBufferSubData(GL_ARRAY_BUFFER, x->vbo_tris_off,
			x->n_frames * x->vbo_frame_size, x->tris);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*
 * carrega texture
 *
 */
void r_teh_model_load_texture(struct teh_model* x)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &x->texture_id);
	glBindTexture(GL_TEXTURE_2D, x->texture_id);
	r_surface_2_texture(x->texture);
	glGenerateMipmap(GL_TEXTURE_2D);
}

/*
 * carrega tudo na gpu
 *
 */
void r_teh_model_load_all(struct teh_model* x)
{
	r_teh_model_load_vbo(x);
	r_teh_model_load_texture(x);
}

/*
 * desenha a parada
 *
 */
void r_teh_model(struct teh_model* x, float w, int f1, int f2, int off, int n)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, x->texture_id);
	glUniform1i(r_texture_sampler_loc, 0);

	glUniform1f(r_vertex_w_loc, w);
	glBindBuffer(GL_ARRAY_BUFFER, x->vbo_id);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
			NULL + x->vbo_tris_off + f1 * x->vbo_frame_size + off);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
			NULL + x->vbo_tris_off + f2 * x->vbo_frame_size + off);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,
			NULL + x->vbo_texcoords_off + off);

	glDrawArrays(GL_TRIANGLES, 0, 3*n);
}

void r_teh_model_at_time(struct teh_model* x, unsigned long t)
{
	int f1, f2;
	float w;

	f1 = ((t * TEH_MODEL_FPS) / 1000) % x->n_frames;
	f2 = (f1 + 1) % x->n_frames;
	w = ((float) (t - f1 * TEH_MODEL_FPS * 1000)) / 1000;

	r_teh_model(x, w, f1, f2, 0, x->n_tris);
}

/*
 * descarrega as parada
 *
 */
void r_teh_model_unload_texture(struct teh_model* x)
{
	glDeleteTextures(1, &x->texture_id);
}

void r_teh_model_unload_vbo(struct teh_model* x)
{
	glDeleteBuffers(1, &x->vbo_id);
	x->vbo_id = 0;
}

void r_teh_model_unload_all(struct teh_model* x)
{
	r_teh_model_unload_texture(x);
	r_teh_model_unload_vbo(x);
}
