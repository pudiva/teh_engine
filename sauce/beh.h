#ifndef HAS_BEH_H
#define HAS_BEH_H

#include <stdbool.h>
#include "teh.h"

struct beh_node
{
	bool is_leaf, is_solid;
	int off, size;
	float plane[4];
	struct beh_node* back, * front;
};

struct beh
{
	struct teh model;

	int n_nodes;
	struct beh_node* nodes;
};

void beh_read(struct beh* bsp, FILE* fp);
void beh_write(struct beh* bsp, FILE* fp);
void beh_read_file(struct beh* bsp, const char* path);
void beh_write_file(struct beh* bsp, const char* path);

#endif
