/*
 * referencias:
 *
 * http://www.cs.utah.edu/~jsnider/SeniorProj/BSP1/default.html
 * https://github.com/id-Software/Quake-III-Arena
 *
 */

#include <stdbool.h>

struct tri
{
	float tc[3][2];
	float v[3][3];
	float plane[4];
	struct tri* next;
};

union node;

struct leaf
{
	bool is_leaf;
	bool is_solid;
	struct tri* tris;
};

struct trunk
{
	bool is_leaf;
	struct tri* splitter;
	union node* kids[2];
};

union node
{
	bool is_leaf;
	struct leaf as_leaf;
	struct trunk as_trunk;
};

/*
 * piscina de triangulos jajaja
 *
 */
#define TRI_POOL_LEN 10000
struct tri tri_pool[TRI_POOL_LEN];
unsigned long tri_pool_c;

struct tri* tri_alloc();
int tri_shallow_split_score(struct tri* restrict x, float* restrict plane);
int tri_deep_split_score(struct tri* restrict x, float* restrict plane);
void tri_shallow_split(
		struct tri* restrict x,
		struct tri* restrict * restrict y,
		float* restrict plane
		);
void tri_deep_split(
		struct tri* restrict x,
		struct tri* restrict * restrict y,
		float* restrict plane
		);
int node_init(union node* node, struct tri* x);
