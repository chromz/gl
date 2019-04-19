// Rodrigo Custodio

#ifndef GL_MATRIX_H
#define GL_MATRIX_H

#include "vector.h"

struct mat4f {
	unsigned rowc;
	unsigned colc;
	float data[16];
};

void mat4f_mul4_set(struct mat4f *res, const struct mat4f *a,
		    const struct mat4f *b);

struct mat4f mat4f_mul4(const struct mat4f *a, const struct mat4f *b);

// Pure laziness
struct vec4 mat4f_mul_vec4(const struct mat4f *a, const struct vec4 *b);

void mat4f_set(struct mat4f *m, const unsigned row, const unsigned col,
	       float val);

float mat4f_get(const struct mat4f *m, const unsigned x, const unsigned y);

struct mat4f mat4f_identity(void);

struct mat4f *mat4f_identity_p(void);

#endif
