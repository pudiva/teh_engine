#ifndef HAS_POLY_H
#define HAS_POLY_H

#include <stdbool.h>
#include "pool.h"

/* classificação de lado */
enum side
{
	ON_PLANE = 1,
	BACK = 2,
	ON_PLANE_AND_BACK = 3,
	FRONT = 4,
	ON_PLANE_AND_FRONT = 5,
	SPLIT = 6,
	ON_PLANT_AND_SPLIT = 7,
};

/* vértice */
struct vert
{
	float pos[3];		/* posição */
	float normal[3];	/* normal do shader */
	float texcoords[2];	/* coordenadas de textura */
	struct vert* next;	/* próximo vértice */
};

/* polígono */
struct poly
{
	float p[4];		/* plano */
	int n_verts;		/* número de vértices */
	struct vert* verts;	/* aponta para o primeiro vértice */
	struct poly* next;	/* aponta para o próximo polígono */
	bool used;		/* compilador: usado como divisor? */
};

/* resultado da comparação entre vértice e plano */
struct vert_cmp 
{
	struct vert* v;		/* vértice comparado */
	const float* p;		/* plano */
	float h;		/* altura */
	int c;			/* lado do plano no qual o vértice está */
	enum side side;		/* lado(s) do plano nos quais o vértice está */
};

/* resultado da comparação entre lado e plano */
struct edge_cmp
{
	struct vert_cmp vc[2];	/* resultado da comparação dos vértices */
	enum side side;		/* lado(s) do plano nos quais o lado está */
};

/* resultado da comparação entre polígono e plano */
struct poly_cmp
{
	struct poly* poly;	/* polígono comparado */
	struct edge_cmp ec[2];	/* resultado da comparação dos lados */
	enum side side;		/* lado(s) do plano nos quais o polígono está */
};

/*
 * PISCINA DE POLÍGONOS!!1!1ONE!
 *
 */
#define POLY_POOL_BUF_LEN 66666
#define VERT_POOL_BUF_LEN (3 * POLY_POOL_BUF_LEN)

extern struct vert vert_pool_buf[VERT_POOL_BUF_LEN];
extern struct poly poly_pool_buf[POLY_POOL_BUF_LEN];

extern struct pool vert_pool;
extern struct pool poly_pool;

#define vert_alloc() pool_alloc(&vert_pool)
#define vert_free(x) pool_free(&vert_pool, (x))
#define vert_pool_init() pool_init(&vert_pool, VERT_POOL_BUF_LEN, sizeof (struct vert), vert_pool_buf)
#define vert_pool_fini() pool_free(&vert_pool)

#define poly_alloc() pool_alloc(&poly_pool)
#define poly_free(x) pool_free(&poly_pool, (x))
#define poly_pool_init() pool_init(&poly_pool, POLY_POOL_BUF_LEN, sizeof (struct poly), poly_pool_buf)
#define poly_pool_fini() pool_fini(&poly_pool)

/* comparação */
void vert_cmp(struct vert_cmp* vc, struct vert* v, const float* p);
void edge_cmp_next(struct edge_cmp* ec);
void edge_cmp(struct edge_cmp* ec, struct vert* v, const float* p);
void poly_cmp(struct poly_cmp* pc, struct poly* poly, const float* p);

/* duplicação */
struct vert* vert_dup(struct vert* v);
struct poly* poly_dup(struct poly* poly);

/* interpolação */
struct vert* vert_lerp(float w, struct vert* a, struct vert* b);

/* divisão por plano */
void edge_split_winding(struct edge_cmp* ec, struct vert** a, struct vert** b);
void poly_split_winding(struct poly_cmp* pc, struct poly** a, struct poly** b);

void edge_split(struct edge_cmp* ec, struct vert** back, struct vert** front);
void poly_split(struct poly_cmp* pc, struct poly** back, struct poly** front);

#endif
