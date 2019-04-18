// Rodrigo Custodio

#ifndef GL_MATRIX_H
#define GL_MATRIX_H

#include "vector.h"

struct mat4f {
	unsigned rowc;
	unsigned colc;
	float data[16];
};

struct mat4f matf_mul4(struct mat4f *a, struct mat4f *b);

struct vec4 matf_mul_vec4(struct mat4f *a, struct vec4 *b);

#endif
