#ifndef HAS_TEH_MODEL_H
#define HAS_TEH_MODEL_H

#include <SDL.h>
#include <SDL_opengles2.h>
#include <stdio.h>
#include <stdint.h>

struct teh_model
{
	unsigned triangle_c;
	unsigned frame_c;
	float (*tex_coord_v)[2];
	float (*vertex_v)[3];

	SDL_Surface* texture;

	/* gl stuff */
	GLuint vbo_id;
	GLuint texture_id;
};

#define TEH_MODEL_FPS 24

#define teh_model_frame_vertex_c(x) \
	((x)->triangle_c * 3)

#define teh_model_vertex_c(x) \
	((x)->frame_c * teh_model_frame_vertex_c(x))

#define teh_model_tex_coord_c(x) \
	teh_model_frame_vertex_c(x)

#define teh_model_tex_coord_sz(x) \
	(teh_model_tex_coord_c(x) * sizeof (float[2]))

#define teh_model_frame_vertex_sz(x) \
	(teh_model_frame_vertex_c(x) * sizeof (float[3]))

#define teh_model_vertex_sz(x) \
	((x)->frame_c * teh_model_frame_vertex_sz(x))

#define teh_model_vbo_tex_coord_off(x) \
	((uintptr_t) 0)

#define teh_model_vbo_vertex_off(x) \
	((uintptr_t) teh_model_tex_coord_sz(x))

#define teh_model_vbo_frame_vertex_off(x, f) \
	((uintptr_t) (teh_model_vbo_vertex_off(x) + (f) * teh_model_frame_vertex_sz(x)))

#define teh_model_vbo_sz(x) \
	((uintptr_t) (teh_model_tex_coord_sz(x) + teh_model_vertex_sz(x)))


void read_teh_model_vertices_stream(struct teh_model* x, FILE* fp);
void read_teh_model_vertices(struct teh_model* x, const char* path);
void read_teh_model_texture(struct teh_model* x, const char* path);

void r_load_teh_model_vbo(struct teh_model* x);
void r_load_teh_model_texture(struct teh_model* x);

void r_teh_model(struct teh_model* x, unsigned long t);

void r_unload_teh_model_texture(struct teh_model* x);
void r_unload_teh_model_vbo(struct teh_model* x);
void r_unload_teh_model(struct teh_model* x);

void free_teh_model_texture(struct teh_model* x);
void free_teh_model_vertices(struct teh_model* x);
void free_teh_model(struct teh_model* x);

#endif
