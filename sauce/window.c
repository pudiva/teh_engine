#include <assert.h>
#include "window.h"

const unsigned window_w = 1024, window_h = 768;

SDL_Window* sdl_window;
SDL_GLContext sdl_gl_context;

unsigned long window_t1, window_t2, window_t3, window_dt;

int window_init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	sdl_window = SDL_CreateWindow(
			"!!!11!1!!1ONE!!",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			window_w,
			window_h,
			SDL_WINDOW_OPENGL
			);

	if (!sdl_window)
		fprintf(stderr, "SDL_GetError(): %s\n", SDL_GetError());

	assert (sdl_window); /* TODO: proper handling */

	sdl_gl_context = SDL_GL_CreateContext(sdl_window);

	if (!sdl_gl_context)
		fprintf(stderr, "SDL_GetError(): %s\n", SDL_GetError());

	assert (sdl_gl_context); /* TODO: proper handling */

	return 0;
}

void window_tic()
{
	window_t1 = window_t2;
	window_t2 = SDL_GetTicks();
	window_dt = window_t2 - window_t1;
}

void window_begin_frames()
{
	window_resume_frames();
}

void window_resume_frames()
{
	/* twice! */
	window_tic();
	window_tic();
}

void window_pause_frames()
{
}

void window_before_frame()
{
}

void window_after_frame()
{
	SDL_GL_SwapWindow(sdl_window);
}

void window_end_frames()
{
}

int window_fini()
{
	SDL_Quit();
	return 0;
}

