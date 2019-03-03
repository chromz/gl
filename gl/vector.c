// Rodrigo Custodio

#include "gl/vector.h"
#include <stdlib.h>



inline float vector_dot(const struct vec3 *a, const struct vec3 *b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

struct vec3 vector_sum(const struct vec3 *a, const struct vec3 *b)
{
	struct vec3 res = {
		.x = a->x + b->x,
		.y = a->y + b->y,
		.z = a->z + b->z,
	};
	return res;
}

struct vec3 vector_cross(const struct vec3 *a, const struct vec3 *b)
{
	struct vec3 res = {
		.x = a->y * b->z - a->z * b->y,
		.y = a->z * b->x - a->x * b->z,
		.z = a->x * b->y - a->y * b->x,
	};
	return res;
}

