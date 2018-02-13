#ifndef HAS_TEH_BSP_H
#define HAS_TEH_BSP_H

#include <stdbool.h>
#include "teh_model.h"

struct teh_bsp_node
{
	bool is_leaf, is_solid;
	int off, size;
	struct teh_bsp_node* back, * front;
};

struct teh_bsp
{
	struct teh_model model;

	int n_nodes;
	struct teh_bsp_node* nodes;
};

void teh_bsp_read(struct teh_bsp* bsp, FILE* fp);
void teh_bsp_write(struct teh_bsp* bsp, FILE* fp);
void teh_bsp_read_file(struct teh_bsp* bsp, const char* path);
void teh_bsp_write_file(struct teh_bsp* bsp, const char* path);

#endif
