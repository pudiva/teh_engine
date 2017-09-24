/*
 * teh main!!11!1ONE
 *
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "window.h"
#include "controller.h"
#include "renderer.h"

/* begin stuff */
#include "teh_model.h"

struct teh_model igualopeople;

void init()
{
	read_teh_model_vertices(&igualopeople, "igualopeople.teh");
	read_teh_model_texture(&igualopeople, "igualopeople.png");

	r_load_teh_model_vbo(&igualopeople);
	r_load_teh_model_texture(&igualopeople);
	free_teh_model(&igualopeople);

	assert (0 < igualopeople.triangle_c);
	assert (0 < igualopeople.frame_c);
}

void frame()
{
	glViewport(0, 0, window_w, window_h);

	float t = ((float) (SDL_GetTicks() % 1000))/1000;
	r_clear(t, t, t, 1);

	//glUniform3f(r_pos_loc, 0, 0, -3);
	//glUniform4f(r_color_loc, .5, .5, .5, 1);

	r_teh_model(&igualopeople, SDL_GetTicks());
}

void fini()
{
	r_unload_teh_model(&igualopeople);
}
/* end stuff */

static bool should_run = true;

static void handle_events()
{
	SDL_Event ev;

	while (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
		case SDL_QUIT:
			puts("Quitting");
			should_run = false;
			break;

		default:
			controller_handle(&ev);
		}
	}
}

int main(int argc, char *argv[])
{
	window_init();
	window_begin_frames();
	renderer_init();

	init();

	while (should_run)
	{
		window_before_frame();
		handle_events();

		frame();

		window_after_frame();
	}

	fini();

	renderer_fini();
	window_fini();

	return 0;
}
