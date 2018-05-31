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

#include "box.h"

/* otimização do ramon */
#define if while

struct teh* igualopeople;
struct teh* teh_boxxy;
struct beh* igualomapa;
struct box* boxxy;

void init()
{
	igualopeople = teh_get("igualopeople.teh");
	assert (igualopeople);

	igualomapa = beh_get("blorl.beh");
	assert (igualomapa);

	teh_boxxy = teh_get("boxxy.teh");
	assert (teh_boxxy);

	igualopeople->texture = image_get("textures/igualopeople.png");
	igualomapa->model.texture = image_get("textures/igualopeople.png");
	teh_boxxy->texture = image_get("textures/boxxy.png");
	assert (teh_boxxy->texture);
	assert (igualopeople->texture);

	r_teh_load_all(igualopeople);
	r_teh_load_all(&igualomapa->model);
	r_teh_load_all(teh_boxxy);

	box_pool_init();

	float boxxy_size[3] = {3, 4, 5};
	float boxxy_pos[3] = {0, 0, 0};
	boxxy = box_new(boxxy_pos, boxxy_size);
}

void fini()
{
}

/*
 * fas o boneco olha com mouse
 *
 */
static float c_look_angles[4] = {-3.14/2, 0, 0, 0};
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

	c_look_angles[0] += .01 * ev->motion.yrel;
	c_look_angles[1]  = 0;
	c_look_angles[2] += .01 * ev->motion.xrel;
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
static float c_eye_front[4] = {0};
static float c_eye_up[4] = {0};

static float c_modelview[4][4] = {0};

/*
 * movimentum
 *
 */
static void look_and_move()
{
	float dt;

	float rot_x[4][4], rot_z[4][4], rot_zx[4][4];
	float front[4] = {0, 0, -1, 0};
	float up[4] = {0, 1, 0, 0};

	/* rotação do zoio */
	mat4_rotate_z(-c_look_angles[2], rot_z);
	mat4_rotate_x(-c_look_angles[0], rot_x);
	mat4_gemm(1, rot_z, rot_x, 0, rot_zx);

	/* frente e cima do zoio */
	mat4_gemv(1, rot_zx, front, 0, c_eye_front);
	mat4_gemv(1, rot_zx, up, 0, c_eye_up);

	/* move a caixa */
	dt = ((float) window_dt)/1000;
	mat4_gemv(5*dt, rot_z, c_walk_dir, 1, boxxy->pos);

	/* move o zoio */
	vec3_copy(boxxy->pos, c_eye_pos);
	vec3_axpy(-5, c_eye_front, c_eye_pos);

	mat4_look_from(c_eye_pos, c_eye_front, c_eye_up, c_modelview);
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
	float T_boxxy[4][4] = {0}, S_boxxy[4][4] = {0}, TS_boxxy[4][4] = {0};
	float t;

	glViewport(0, 0, window_w, window_h);

	t = ((float) (SDL_GetTicks() % 1000))/1000;
	r_clear(t * (178.0/255), t * (102.0/255), t * (131.0/255), 1);

	look_and_move();

	glClearDepthf(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

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

	glDepthFunc(GL_LESS);

	mat4_gemm(1, c_modelview, T_igualopeople, 0, M);
	r_modelview(M[0]);
	r_teh_at_time(igualopeople, SDL_GetTicks());

	mat4_translate(boxxy->pos, T_boxxy);
	mat4_scale(boxxy->size, S_boxxy);

	mat4_gemm(1, T_boxxy, S_boxxy, 0, TS_boxxy);
	mat4_gemm(1, c_modelview, TS_boxxy, 0, M);

	r_modelview(M[0]);
	r_teh_at_time(teh_boxxy, 1);
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
