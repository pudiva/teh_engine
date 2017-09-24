#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "renderer.h"
#include "teh_model.h"

#include <SDL_image.h>

/*
 * read
 *
 */
void read_teh_model_vertices_stream(struct teh_model* x, FILE* fp)
{
	int i, r;
	memset(x, 0, sizeof (*x));

	fscanf(fp, "dota? =op\n");
	fscanf(
			fp,
			"teh model has %d triangles and %d frames.\n",
			&x->triangle_c,
			&x->frame_c
	      );

	x->tex_coord_v = malloc(teh_model_tex_coord_sz(x));
	x->vertex_v = malloc(teh_model_vertex_sz(x));

	fscanf(fp, "teh texcoords are\n");
	for (i = 0; i < teh_model_tex_coord_c(x); ++i)
	{
		r = fscanf(
				fp,
				"%f %f\n",
				&x->tex_coord_v[i][0],
				&x->tex_coord_v[i][1]
			  );
		assert (r == 2);
	}

	fscanf(fp, "and teh vertices are\n");
	for (i = 0; i < teh_model_vertex_c(x); ++i)
	{
		r = fscanf(
				fp,
				"%f %f %f\n",
				&x->vertex_v[i][0],
				&x->vertex_v[i][1],
				&x->vertex_v[i][2]
			  );
		assert (r == 3);
	}
}

void read_teh_model_vertices(struct teh_model* x, const char* path)
{
	FILE* fp;
	fp = fopen(path, "r");
	assert (fp);
	read_teh_model_vertices_stream(x, fp);
	fclose(fp);
}

void read_teh_model_texture(struct teh_model* x, const char* path)
{
	x->texture = IMG_Load(path);
	assert (x->texture);
}

/*
 * load
 *
 */
void r_load_teh_model_vbo(struct teh_model* x)
{
	glGenBuffers(1, &x->vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, x->vbo_id);

	glBufferData(
			GL_ARRAY_BUFFER,
			teh_model_vbo_sz(x),
			NULL,
			GL_STATIC_DRAW
		    );

	glBufferSubData(
			GL_ARRAY_BUFFER,
			teh_model_vbo_tex_coord_off(x),
			teh_model_tex_coord_sz(x),
			x->tex_coord_v
		       );

	glBufferSubData(
			GL_ARRAY_BUFFER,
			teh_model_vbo_vertex_off(x),
			teh_model_vertex_sz(x),
			x->vertex_v
		       );

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void r_load_teh_model_texture(struct teh_model* x)
{
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures(1, &x->texture_id);
	glBindTexture(GL_TEXTURE_2D, x->texture_id);
	r_surface_2_texture(x->texture);
	glGenerateMipmap(GL_TEXTURE_2D);
}

/*
 * draw
 *
 */
void r_teh_model(struct teh_model* x, unsigned long t)
{
	int f1, f2;
	float w;
	const GLvoid* v1_off, * v2_off, * tc_off;

	f1 = ((t * TEH_MODEL_FPS) / 1000) % x->frame_c;
	f2 = (f1 + 1) % x->frame_c;
	w = ((float) (t - f1 * TEH_MODEL_FPS * 1000)) / 1000;

	v1_off = (const GLvoid*) teh_model_vbo_frame_vertex_off(x, f1);
	v2_off = (const GLvoid*) teh_model_vbo_frame_vertex_off(x, f2);
	tc_off = (const GLvoid*) teh_model_vbo_tex_coord_off(x);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, x->texture_id);
	glUniform1i(r_texture_sampler_loc, 0);

	glUniform1f(r_vertex_w_loc, w);
	glBindBuffer(GL_ARRAY_BUFFER, x->vbo_id);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, v1_off);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, v2_off);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, tc_off);

	glDrawArrays(GL_TRIANGLES, 0, 3 * x->triangle_c);
}

/*
 * unload
 *
 */
void r_unload_teh_model_texture(struct teh_model* x)
{
	glDeleteTextures(1, &x->texture_id);
}

void r_unload_teh_model_vbo(struct teh_model* x)
{
	glDeleteBuffers(1, &x->vbo_id);
	x->vbo_id = 0;
}

void r_unload_teh_model(struct teh_model* x)
{
	r_unload_teh_model_texture(x);
	r_unload_teh_model_vbo(x);
}

/*
 * free
 *
 */
void free_teh_model_texture(struct teh_model* x)
{
	SDL_FreeSurface(x->texture);
}

void free_teh_model_vertices(struct teh_model* x)
{
	free(x->vertex_v);
	free(x->tex_coord_v);
}

void free_teh_model(struct teh_model* x)
{
	free_teh_model_texture(x);
	free_teh_model_vertices(x);
}
