/*
 * teh main!!11!1ONE
 *
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "window.h"
#include "renderer.h"

/* begin stuff */
#include "assets.h"
#include "r_teh.h"
#include "r_beh.h"
#include "vec.h"

struct teh* igualopeople;
struct beh* igualobsp;

void init()
{
	igualopeople = teh_get("igualopeople.teh");
	assert (igualopeople);

	igualobsp = beh_get("igualopeople.beh");
	assert (igualobsp);

	igualopeople->texture = image_get("igualopeople.png");
	igualobsp->model.texture = image_get("igualopeople.png");
	assert (igualopeople->texture);

	r_teh_load_all(igualopeople);
	r_teh_load_all(&igualobsp->model);
}

void fini()
{
}

void frame()
{
	glViewport(0, 0, window_w, window_h);

	float t = ((float) (SDL_GetTicks() % 1000))/1000;
	r_clear(t * (178.0/255), t * (102.0/255), t * (131.0/255), 1);

	//glUniform3f(r_pos_loc, 0, 0, -3);
	//glUniform4f(r_color_loc, .5, .5, .5, 1);

	//r_teh_at_time(igualopeople, SDL_GetTicks());
	//r_teh_at_time(&igualobsp->model, SDL_GetTicks());
	r_beh_from_eye(igualobsp, zero3);
}

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
