#include <assert.h>
#include "trace.h"
#include "vec.h"

/* TODO: pilha explicita? */
void tr_beh_node(struct trace* tr, struct beh_node* node)
{
	float h, h_near;
	float dh;
	float cmp, cmp_near;
	float v_h;
	float t_last;
	int i_kid, i_verts_near;

	switch (node->type)
	{
	/* tem mais nada aqui */
	case LEAF:
		break;

	/* POW */
	case SOLID_LEAF:
		if (tr->t < tr->t_best)
		{
			tr->t_best = tr->t;
			tr->node_best = node;
		}
		break;

	/* testa as qiança */
	case NON_LEAF:
		h = vec3_dot(tr->b->pos, node->plane);
		dh = node->plane[3] - h;

		i_verts_near =
			(node->plane[0] * dh < 0) << 0 |
			(node->plane[1] * dh < 0) << 1 |
			(node->plane[2] * dh < 0) << 2;

		h_near = vec3_dot(tr->b->verts[i_verts_near], node->plane);

		cmp = vec_cmp(h, node->plane[3]);
		cmp_near = vec_cmp(h_near, node->plane[3]);

		i_kid = cmp <= 0 ? 0 : 1;

		/* entra nesse lado de qq jeito */
		tr_beh_node(tr, node->kids[i_kid]);

		/* ta nos 2 lado */
		if (cmp != cmp_near || cmp == 0)
		{
			tr_beh_node(tr, node->kids[1-i_kid]);
		}

		/* tenta colisão com plano, se ainda tiver tempo */
		else if (tr->t < tr->t_max)
		{
			/* calcula tempo pra colisão com vértice mais próximo */
			v_h = vec3_dot(tr->b->verts[i_verts_near], node->plane);

			/* movimento colide com plano */
			if (vec_cmp(v_h, 0) != 0)
			{
				t_last = tr->t;
				tr->t = (dh) / v_h;

				/* intervalo válido */
				if (t_last < tr->t && tr->t < tr->t_max)
					tr_beh_node(tr, node->kids[1-i_kid]);
			}
		}
		break;

	default:
		assert (false); /* BUGADO */
	}
}

void tr_beh(struct trace* tr, struct beh* beh)
{
	tr_beh_node(tr, beh->nodes);
}

