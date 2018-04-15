#ifndef HAS_POOL_H
#define HAS_POOL_H

#include <stdbool.h>

/* piscina jajaja */
struct pool
{
	int size;
	int n_memb;
	int n_free;
	int n_used;
	int n_max_used;
	bool is_buf_owned;
	void* free;
	void* buf;
};

/* esvazia piscina */
void pool_clear(struct pool* p);

/* inicializa piscina */
void pool_init(struct pool* p, int n_memb, int size, void* buf);

/* pega elemento da piscina */
void* pool_alloc(struct pool* p);

/* devolve elemento da piscina */
void pool_free(struct pool* p, void* x);

/* finaliza piscina */
void pool_fini(struct pool* p);

#endif
