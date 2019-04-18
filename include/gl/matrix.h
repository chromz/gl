// Rodrigo Custodio

#ifndef GL_MATRIX_H
#define GL_MATRIX_H

#include "vector.h"

struct matf {
	unsigned rowc;
	unsigned colc;
	float *data;
};

struct matf *matf_mul4(struct matf *a, struct matf *b);

struct vec4 *matf_mul_vec4(struct matf *a, struct vec4 *b);

void matf_free(struct matf *m);


#endif
