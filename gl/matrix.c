// Rodrigo Custodio

#include "gl/matrix.h"

#include <stdlib.h>


struct matf *matf_mul4(struct matf *a, struct matf *b)
{
	if (a->rowc != 4 && a->colc !=4  && a->rowc != b->rowc &&
	    a->colc != b->rowc) {
		return NULL;
	}
	struct matf *res = malloc(sizeof(*res));
	res->rowc = a->rowc;
	res->colc = a->colc;

	res->data = malloc(sizeof(*res->data) * res->rowc * res->colc);

	res->data[0] = a->data[0] * b->data[0] + a->data[1] * b->data[4] +
		       a->data[2] * b->data[8] + a->data[3] * b->data[12];
	res->data[1] = a->data[0] * b->data[1] + a->data[1] * b->data[5] +
		       a->data[2] * b->data[9] + a->data[3] * b->data[13];
	res->data[2] = a->data[0] * b->data[2] + a->data[1] * b->data[6] +
		       a->data[2] * b->data[10] + a->data[3] * b->data[14];
	res->data[3] = a->data[0] * b->data[3] + a->data[1] * b->data[7] +
		       a->data[2] * b->data[11] + a->data[3] * b->data[15];

	res->data[4] = a->data[4] * b->data[0] + a->data[5] * b->data[4] +
		       a->data[6] * b->data[8] + a->data[7] * b->data[12];
	res->data[5] = a->data[4] * b->data[1] + a->data[5] * b->data[5] +
		       a->data[6] * b->data[9] + a->data[7] * b->data[13];
	res->data[6] = a->data[4] * b->data[2] + a->data[5] * b->data[6] +
		       a->data[6] * b->data[10] + a->data[7] * b->data[14];
	res->data[7] = a->data[4] * b->data[3] + a->data[5] * b->data[7] +
		       a->data[6] * b->data[11] + a->data[7] * b->data[15];

	res->data[8] = a->data[8] * b->data[0] + a->data[9] * b->data[4] +
		       a->data[10] * b->data[8] + a->data[11] * b->data[12];
	res->data[9] = a->data[8] * b->data[1] + a->data[9] * b->data[5] +
		       a->data[10] * b->data[9] + a->data[11] * b->data[13];
	res->data[10] = a->data[8] * b->data[2] + a->data[9] * b->data[6] +
		       a->data[10] * b->data[10] + a->data[11] * b->data[14];
	res->data[11] = a->data[8] * b->data[3] + a->data[9] * b->data[7] +
		       a->data[10] * b->data[11] + a->data[11] * b->data[15];
	
	res->data[12] = a->data[12] * b->data[0] + a->data[13] * b->data[4] +
		       a->data[14] * b->data[8] + a->data[15] * b->data[12];
	res->data[13] = a->data[12] * b->data[1] + a->data[13] * b->data[5] +
		       a->data[14] * b->data[9] + a->data[15] * b->data[13];
	res->data[14] = a->data[12] * b->data[2] + a->data[13] * b->data[6] +
		       a->data[14] * b->data[10] + a->data[15] * b->data[14];
	res->data[15] = a->data[12] * b->data[3] + a->data[13] * b->data[7] +
		       a->data[14] * b->data[11] + a->data[15] * b->data[15];
	return res;
}

struct vec4 *matf_mul_vec4(struct matf *a, struct vec4 *b)
{
	if (a->rowc != 4 && a->colc != 4) {
		return NULL;
	}
	struct vec4 *res = malloc(sizeof(*res));
	res->x = a->data[0] * b->x + a->data[1] * b->y + a->data[2] * b->z +
		 a->data[3] * b->w;
	res->y = a->data[4] * b->x + a->data[5] * b->y + a->data[6] * b->z +
		 a->data[7] * b->w;
	res->z = a->data[8] * b->x + a->data[9] * b->y + a->data[10] * b->z +
		 a->data[11] * b->w;
	res->w = a->data[12] * b->x + a->data[13] * b->y + a->data[14] * b->z +
		 a->data[15] * b->w;
	return res;
}

void matf_free(struct matf *m)
{
	free(m->data);
	free(m);
}
