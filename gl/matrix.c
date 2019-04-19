// Rodrigo Custodio

#include "gl/matrix.h"

#include <stdlib.h>


void mat4f_mul4_set(struct mat4f *res, const struct mat4f *a,
		    const struct mat4f *b)
{
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
}

struct mat4f mat4f_mul4(const struct mat4f *a, const struct mat4f *b)
{
	struct mat4f res;
	res.rowc = a->rowc;
	res.colc = a->colc;
	mat4f_mul4_set(&res, a, b);
	return res;
}

struct vec4 mat4f_mul_vec4(const struct mat4f *a, const struct vec4 *b)
{
	struct vec4 res;
	res.x = a->data[0] * b->x + a->data[1] * b->y + a->data[2] * b->z +
		a->data[3] * b->w;
	res.y = a->data[4] * b->x + a->data[5] * b->y + a->data[6] * b->z +
		a->data[7] * b->w;
	res.z = a->data[8] * b->x + a->data[9] * b->y + a->data[10] * b->z +
		a->data[11] * b->w;
	res.w = a->data[12] * b->x + a->data[13] * b->y + a->data[14] * b->z +
		a->data[15] * b->w;
	return res;
}

inline void mat4f_set(struct mat4f *m, const unsigned row, const unsigned col,
		      float val)
{
	m->data[row * m->rowc + col] = val;
}

inline float mat4f_get(const struct mat4f *m,
		       const unsigned x, const unsigned y)
{
	return m->data[y * m->rowc + x];
}

inline struct mat4f mat4f_identity(void)
{
	return (struct mat4f) {
		.rowc = 4,
		.colc = 4,
		.data = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		}
	};
}

struct mat4f *mat4f_identity_p(void)
{
	struct mat4f *identity = malloc(sizeof(*identity));
	identity->rowc = 4;
	identity->colc = 4;
	identity->data[0] = 1;
	for (int i = 1; i < 4; i++) {
		identity->data[i] = 0;
	}
	identity->data[4] = 0;
	identity->data[5] = 1;
	identity->data[8] = 0;
	identity->data[9] = 0;
	identity->data[10] = 1;
	identity->data[11] = 0;
	for (int i = 12; i < 15; i++) {
		identity->data[i] = 0;
	}
	identity->data[15] = 1;
	return identity;
}

