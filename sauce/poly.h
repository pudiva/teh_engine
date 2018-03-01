#ifndef HAS_POLY_H
#define HAS_POLY_H

#include <stdbool.h>

enum side
{
	ON_PLANE = 1,
	BACK = 2,
	FRONT = 4,
	SPLIT = 6,
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
};

/* resultado da comparação entre vértice e plano */
struct vert_cmp 
{
	const struct vert* v;	/* vértice comparado */
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

void vert_cmp(struct vert_cmp* vc, const struct vert* v, const float* p);
void edge_cmp_next(struct edge_cmp* ec);
void edge_cmp(struct edge_cmp* ec, const struct vert* v, const float* p);
void poly_cmp(struct poly_cmp* pc, struct poly* poly, const float* p);

#endif
