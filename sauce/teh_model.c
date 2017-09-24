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

GLuint CreateSimpleTexture2D( )
{
   // Texture object handle
   GLuint textureId;
   
   // 2x2 Image, 3 bytes per pixel (R, G, B)
   GLubyte pixels[4 * 3] =
   {  
      255,   0,   0, // Red
        0, 255,   0, // Green
        0,   0, 255, // Blue
      255, 255,   0  // Yellow
   };

   // Use tightly packed data
   glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

   // Generate a texture object
   glGenTextures ( 1, &textureId );

   // Bind the texture object
   glBindTexture ( GL_TEXTURE_2D, textureId );

   // Load the texture
   glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

   // Set the filtering mode
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   return textureId;
}

void r_load_teh_model_texture(struct teh_model* x)
{
	glEnable(GL_TEXTURE_2D);

	x->texture_id = CreateSimpleTexture2D();

	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures(1, &x->texture_id);
	glBindTexture(GL_TEXTURE_2D, x->texture_id);
	r_surface_2_texture(x->texture);

	/* NOTE: sem isso o model fica preto */
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
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

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, x->texture_id);
	glUniform1i(r_texture_sampler_loc, 0);

	glUniform1f(r_vertex_w_loc, w);
	glBindBuffer(GL_ARRAY_BUFFER, x->vbo_id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, v1_off);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, v2_off);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, tc_off);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glDrawArrays(GL_TRIANGLES, 0, 3 * x->triangle_c);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisable(GL_TEXTURE_2D);
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
