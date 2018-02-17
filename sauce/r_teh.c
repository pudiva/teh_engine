#include "renderer.h"
#include "r_teh.h"

/*
 * carrega VBO
 *
 */
void r_teh_load_vbo(struct teh* x)
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
void r_teh_load_texture(struct teh* x)
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
void r_teh_load_all(struct teh* x)
{
	r_teh_load_vbo(x);
	r_teh_load_texture(x);
}

/*
 * desenha a parada
 *
 */
void r_teh(struct teh* x, float w, int f1, int f2, int off, int n)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, x->texture_id);
	glUniform1i(r_texture_sampler_loc, 0);

	glUniform1f(r_vertex_w_loc, w);
	glBindBuffer(GL_ARRAY_BUFFER, x->vbo_id);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
			NULL + x->vbo_tris_off + f1 * x->vbo_frame_size + off * sizeof (float[3][3]));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
			NULL + x->vbo_tris_off + f2 * x->vbo_frame_size + off * sizeof (float[3][3]));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,
			NULL + x->vbo_texcoords_off + off * sizeof (float[3][2]));

	glDrawArrays(GL_TRIANGLES, 0, 3*n);
}

void r_teh_at_time(struct teh* x, unsigned long t)
{
	int f1, f2;
	float w;

	f1 = ((t * TEH_FPS) / 1000) % x->n_frames;
	f2 = (f1 + 1) % x->n_frames;
	w = ((float) (t - f1 * TEH_FPS * 1000)) / 1000;

	r_teh(x, w, f1, f2, 0, x->n_tris);
}

/*
 * descarrega as parada
 *
 */
void r_teh_unload_texture(struct teh* x)
{
	glDeleteTextures(1, &x->texture_id);
}

void r_teh_unload_vbo(struct teh* x)
{
	glDeleteBuffers(1, &x->vbo_id);
	x->vbo_id = 0;
}

void r_teh_unload_all(struct teh* x)
{
	r_teh_unload_texture(x);
	r_teh_unload_vbo(x);
}
