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

#include <SDL_image.h>

/* begin stuff */
#include "teh_model.h"

struct teh_model igualopeople;

void init()
{
	teh_model_read_file(&igualopeople, "igualopeople.teh_model");
	igualopeople.texture = IMG_Load("igualopeople.png");
	assert (igualopeople.texture);

	r_teh_model_load_vbo(&igualopeople);
	r_teh_model_load_texture(&igualopeople);

	teh_model_free(&igualopeople);
	SDL_FreeSurface(igualopeople.texture);

	/* FIXME: escrever testes decentemente */
	assert (0 < igualopeople.n_tris);
	assert (0 < igualopeople.n_frames);
}

void frame()
{
	glViewport(0, 0, window_w, window_h);

	float t = ((float) (SDL_GetTicks() % 1000))/1000;
	r_clear(t, t, t, 1);

	//glUniform3f(r_pos_loc, 0, 0, -3);
	//glUniform4f(r_color_loc, .5, .5, .5, 1);

	r_teh_model_at_time(&igualopeople, SDL_GetTicks());
}

void fini()
{
	r_teh_model_unload_all(&igualopeople);
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
