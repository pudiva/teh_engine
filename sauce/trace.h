#ifndef HAS_TRACE_H
#define HAS_TRACE_H

#include "box.h"
#include "beh.h"

struct trace
{
	struct box* b;
	float v[3];
	float t_min, t_max;

	float t;
	float t_best;
	struct beh_node* node_best;
};

void tr_beh_node(struct trace* tr, struct beh_node* node);
void tr_beh(struct trace* tr, struct beh* beh);

#endif
