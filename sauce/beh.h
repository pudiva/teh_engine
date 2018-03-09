#ifndef HAS_BEH_H
#define HAS_BEH_H

#include <stdbool.h>
#include "teh.h"

/* tipos de nodo */
enum beh_node_type
{
	NON_LEAF = 0,
	LEAF = 1,
	LIQUID_LEAF = 1,
	SOLID_LEAF = 3,
};

/* nodo bsp */
struct beh_node
{
	enum beh_node_type type;	/* tipo de nodo */
	float plane[4];			/* plano divisor */
	struct beh_node* kids[2];	/* as quiança */
	int i[2];			/* os indice no teh model */
	struct poly* polys;		/* compilador: os poligono */
};

struct beh
{
	struct teh model;		/* teh model */
	int n_nodes;			/* número de nodos */
	struct beh_node* nodes;		/* nodos */
};

void beh_read(struct beh* bsp, FILE* fp);
void beh_write(struct beh* bsp, FILE* fp);
void beh_read_file(struct beh* bsp, const char* path);
void beh_write_file(struct beh* bsp, const char* path);

#endif
