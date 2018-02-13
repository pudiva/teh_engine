#ifndef HAS_TEH_MODEL_H
#define HAS_TEH_MODEL_H

#include <SDL.h>
#include <SDL_opengles2.h>
#include <stdio.h>
#include <stdint.h>

struct teh_model
{
	unsigned n_tris;
	unsigned n_frames;
	float (*texcoords)[3][2];
	float (*tris)[3][3];

	SDL_Surface* texture;

	/* gl stuff */
	GLuint vbo_id;
	GLuint texture_id;

	GLintptr vbo_texcoords_off;
	GLintptr vbo_tris_off;
	GLsizeiptr vbo_frame_size;
};

#define TEH_MODEL_FPS 24

void teh_model_read(struct teh_model* x, FILE* fp);
void teh_model_read_file(struct teh_model* x, const char* path);
void teh_model_free(struct teh_model* x);

void r_teh_model_load_vbo(struct teh_model* x);
void r_teh_model_load_texture(struct teh_model* x);
void r_teh_model_load_all(struct teh_model* x);
void r_teh_model_at_time(struct teh_model* x, unsigned long t);
void r_teh_model(struct teh_model* x, float w, int f1, int f2, int off, int n);
void r_teh_model_unload_texture(struct teh_model* x);
void r_teh_model_unload_vbo(struct teh_model* x);
void r_teh_model_unload_all(struct teh_model* x);

#endif
