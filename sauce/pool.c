#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "pool.h"

#define P(p, i) ((void**) ((p)->buf + (i) * (p)->size))

void pool_clear(struct pool* p)
{
	int i;

	for (i = 0; i < p->n_memb; ++i)
		(*P(p, i)) = P(p, i+1);

	(*P(p, i-1)) = NULL;
	p->free = p->buf;
	p->n_free = p->n_memb;
	p->n_used = 0;
}

void pool_init(struct pool* p, int n_memb, int size, void* buf)
{
	assert (p);
	assert (0 <= n_memb);
	assert (sizeof (void*) <= size);

	memset(p, 0, sizeof (struct pool));

	p->size = size;
	p->n_memb = n_memb;

	if (buf)
	{
		p->buf = buf;
	}

	else
	{
		p->buf = calloc(n_memb, size);
		p->is_buf_owned = true;
	}

	pool_clear(p);
}

void* pool_alloc(struct pool* p)
{
	void* x;

	assert (p);

	x = p->free;
	assert (x);

	p->free = (*((void**) x));
	--p->n_free;
	++p->n_used;

	if (p->n_max_used < p->n_used)
		p->n_max_used = p->n_used;

	return x;
}

void pool_free(struct pool* p, void* x)
{
	/* NOTE: não protege de free duplo */

	assert (p); /* melhor falhar aqui do que dentro do if */

	if (x)
	{
		assert ((x - p->buf) % p->size == 0);
		assert ((x - p->buf) / p->size < p->n_memb);

		(*((void**) x)) = p->free;
		p->free = x;
		
		++p->n_free;
		--p->n_used;
	}
}

void pool_fini(struct pool* p)
{
	if (p->is_buf_owned)
	{
		free(p->buf);
	}
}
