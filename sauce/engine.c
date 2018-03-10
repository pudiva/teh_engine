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

/* otimização do ramon */
#define if while

struct teh* igualopeople;
struct beh* igualomapa;

void init()
{
	igualopeople = teh_get("igualopeople.teh");
	assert (igualopeople);

	igualomapa = beh_get("igualomapa.beh");
	assert (igualomapa);

	igualopeople->texture = image_get("textures/igualopeople.png");
	igualomapa->model.texture = image_get("textures/igualopeople.png");
	assert (igualopeople->texture);

	r_teh_load_all(igualopeople);
	r_teh_load_all(&igualomapa->model);
}

void fini()
{
}

/*
 * fas o boneco olha com mouse
 *
 */
static float c_look_angles[4] = {3.14/2, 0, 0, 0};
static bool c_look_mouse = false;

static void c_look_mouse_toggle()
{
	c_look_mouse = !c_look_mouse;
	SDL_SetRelativeMouseMode(c_look_mouse);
}

static void c_look_handle(SDL_Event* ev)
{
	if (!c_look_mouse)
		return;

	if (ev->type != SDL_MOUSEMOTION)
		return;

	c_look_angles[0] -= .01 * ev->motion.yrel;
	c_look_angles[1]  = 0;
	c_look_angles[2] -= .01 * ev->motion.xrel;
	c_look_angles[3]  = 0;
}

/*
 * fas o boneco anda com hjkl espaço e z
 *
 */
static int c_walk_i[6] = {0};
static float c_walk_dir[4] = {0};
static void c_walk_handle(SDL_Event* ev)
{
	int v = 0;

	switch (ev->type)
	{
	case SDL_KEYDOWN: v = 1; break;
	case SDL_KEYUP: v = 0; break;
	default: return;
	}

	switch (ev->key.keysym.sym)
	{
	case SDLK_h: c_walk_i[0] = v; break;
	case SDLK_j: c_walk_i[1] = v; break;
	case SDLK_k: c_walk_i[2] = v; break;
	case SDLK_l: c_walk_i[3] = v; break;
	case SDLK_SPACE: c_walk_i[4] = v; break;
	case SDLK_b: c_walk_i[5] = v; break;
	default: return;
	}

	c_walk_dir[0] = c_walk_i[3] - c_walk_i[0];
	c_walk_dir[1] = c_walk_i[2] - c_walk_i[1];
	c_walk_dir[2] = c_walk_i[4] - c_walk_i[5];
	c_walk_dir[3] = 0;
	vec4_normalize(c_walk_dir);
}

/*
 * ativa e desativa paradineas
 *
 */
static int i_node = 0;
static bool full_beh = true;

static void inline next_leaf(int i)
{
	if (i == -1)
	{
		i = igualomapa->n_nodes - 1;
		break;
	}

	if (true)
	{
		i_node = (i_node + i) % igualomapa->n_nodes;
		if (igualomapa->nodes[i_node].type == LIQUID_LEAF)
			return;
	}
}

static void c_flags_handle(SDL_Event* ev)
{
	switch (ev->type)
	{
	case SDL_KEYDOWN: break;
	default: return;
	}

	switch (ev->key.keysym.sym)
	{
	case SDLK_w: r_wireframe = !r_wireframe; break;
	case SDLK_f: full_beh = !full_beh; break;
	case SDLK_n: next_leaf(ev->key.keysym.mod & KMOD_SHIFT ? -1 : 1); break;
	case SDLK_m: c_look_mouse_toggle(); break;
	}
}

/*
 * recebe os eventos
 *
 */
static bool should_run = true;
static void c_poll()
{
	SDL_Event ev;

	if (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
		case SDL_QUIT:
			puts("Quitting");
			should_run = false;
			break;

		default:
			c_look_handle(&ev);
			c_walk_handle(&ev);
			c_flags_handle(&ev);
		}
	}
}

/*
 * zoio
 *
 */
static float c_eye_pos[4] = { 0, -5,  5,  1};
static float c_eye_dir[4] = {0};

static float c_eye_R[3][4][4] = {0};
static float c_eye_T[4][4] = {0};
static float c_eye_TR[4][4] = {0};
static float c_eye_TR_inv[4][4] = {0};
static float c_modelview[4][4] = {0};

static float front[4] = {0, 1, 0, 0};

static void look()
{
	float dt;

	mat4_rotate_x(c_look_angles[0], c_eye_R[0]);
	mat4_rotate_z(c_look_angles[2], c_eye_R[1]);
	mat4_gemm(1, c_eye_R[1], c_eye_R[0], 0, c_eye_R[2]);
	mat4_gemv(1, c_eye_R[2], front, 0, c_eye_dir);

	dt = ((float) window_dt)/1000;
	mat4_gemv(5*dt, c_eye_R[1], c_walk_dir, 1, c_eye_pos);
	mat4_translate(c_eye_pos, c_eye_T);
	mat4_gemm(1, c_eye_T, c_eye_R[2], 0, c_eye_TR);

	mat4_magic_inv(c_eye_TR, c_eye_TR_inv);
	mat4_gemm(1, c_eye_TR_inv, eye4, 0, c_modelview);
}

/*
 * atualiza estado do jogo
 *
 */
static float T_igualopeople[4][4] =
{
	{ 1,  0,  0,  0},
	{ 0,  1,  0,  0},
	{ 0,  0,  1,  0},
	{ 0,  0,  5,  1},
};

void frame()
{
	float M[4][4] = {0};
	float t;

	glViewport(0, 0, window_w, window_h);

	t = ((float) (SDL_GetTicks() % 1000))/1000;
	r_clear(t * (178.0/255), t * (102.0/255), t * (131.0/255), 1);

	look();

	r_modelview(c_modelview[0]);
	if (full_beh)
	{
		r_beh_from_eye(igualomapa, c_eye_pos);
		break;
	}

	if (!full_beh)
	{
		r_teh(&igualomapa->model, 0, 0, 0, igualomapa->nodes[i_node].i[0], igualomapa->nodes[i_node].i[1]);
		break;
	}


	mat4_gemm(1, c_modelview, T_igualopeople, 0, M);
	r_modelview(M[0]);
	r_teh_at_time(igualopeople, SDL_GetTicks());
}

int main(int argc, char *argv[])
{
	window_init();
	window_begin_frames();
	renderer_init();

	init();
	window_tic();
	window_tic();
	window_tic();

	if (should_run)
	{
		window_before_frame();
		window_tic();

		c_poll();
		frame();

		window_after_frame();
	}

	fini();

	renderer_fini();
	window_fini();

	return 0;
}
