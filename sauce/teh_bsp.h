#include <stdbool.h>

struct teh_bsp_leaf
{
	bool is_leaf;
	struct tri* tris;
}

struct teh_bsp_trunk
{
	bool is_leaf;
}

union teh_bsp_node
{
	bool is_leaf;
	struct teh_bsp_leaf as_leaf;
	struct teh_bsp_trunk as_trunk;
}

