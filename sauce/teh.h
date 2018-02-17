#ifndef HAS_TEH_H
#define HAS_TEH_H

#include <SDL.h>
#include <SDL_opengles2.h>
#include <stdio.h>
#include <stdint.h>

struct teh
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

#define TEH_FPS 24

void teh_read(struct teh* x, FILE* fp);
void teh_read_file(struct teh* x, const char* path);
void teh_write(struct teh* x, FILE* fp);
void teh_write_file(struct teh* x, const char* path);

void teh_free(struct teh* x);

void r_teh_load_vbo(struct teh* x);
void r_teh_load_texture(struct teh* x);
void r_teh_load_all(struct teh* x);
void r_teh_at_time(struct teh* x, unsigned long t);
void r_teh(struct teh* x, float w, int f1, int f2, int off, int n);
void r_teh_unload_texture(struct teh* x);
void r_teh_unload_vbo(struct teh* x);
void r_teh_unload_all(struct teh* x);

#endif
