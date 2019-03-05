// Rodrigo Custodio

#include "gl/vector.h"
#include <math.h>
#include <stdlib.h>



inline float vec3_dot(const struct vec3 *a, const struct vec3 *b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

float vec3_norm(const struct vec3 *a)
{
	return sqrtf(powf(a->x, 2.0) + powf(a->y, 2.0) + powf(a->z, 2.0));
}

struct vec3 vec3_sum(const struct vec3 *a, const struct vec3 *b)
{
	return (struct vec3) {
		.x = a->x + b->x,
		.y = a->y + b->y,
		.z = a->z + b->z,
	};
}

struct vec3 vec3_sub(const struct vec3 *a, const struct vec3 *b)
{
	return (struct vec3) {
		.x = a->x - b->x,
		.y = a->y - b->y,
		.z = a->z - b->z,
	};
}

struct vec3 vec3_cross(const struct vec3 *a, const struct vec3 *b)
{
	return (struct vec3) {
		.x = a->y * b->z - a->z * b->y,
		.y = a->z * b->x - a->x * b->z,
		.z = a->x * b->y - a->y * b->x,
	};
}

struct vec3 vec3_normalize(const struct vec3 *a)
{
	float norm = vec3_norm(a);
	return (struct vec3) {
		.x = a->x / norm,
		.y = a->y / norm,
		.z = a->z / norm,
	};
}


