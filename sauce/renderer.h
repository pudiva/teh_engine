#ifndef HAS_RENDERER_H
#define HAS_RENDERER_H

#include <SDL_opengles2.h>
#include "teh.h"
#include <stdbool.h>

extern GLuint r_vertex_shader_id, r_fragment_shader_id, r_program_id;
extern GLint r_pos_loc, r_color_loc, r_vertex_w_loc, r_modelview_loc, r_texture_sampler_loc;
extern bool r_wireframe;

extern unsigned char r_vertex_shader_glsl[], r_fragment_shader_glsl[];
extern unsigned int r_vertex_shader_glsl_len, r_fragment_shader_glsl_len;

void renderer_init();
void r_surface_2_texture(SDL_Surface* img);
void renderer_fini();

#define r_pos3fv(v) glUniform3fv(r_pos_loc, 1, (v))
#define r_color4fv(v) glUniform4fv(r_color_loc, 1, (v))
#define r_modelview(A) glUniformMatrix4fv(r_modelview_loc, 1, false, (A))
#define r_clear(r, g, b, a) \
{ \
	glClearColor(r, g, b, a); \
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); \
}

#endif
