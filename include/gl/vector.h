// Rodrigo Custodio

#ifndef GL_VECTOR_H
#define GL_VECTOR_H

struct dvec4 {
	double x, y, z, w;
};

struct vec4 {
	float x, y, z, w;
};

struct vec4i {
	int x, y, z, w;
};

struct vec4u {
	unsigned x, y, z, w;
};

struct dvec3 {
	double x, y, z;
};

struct vec3i {
	int x, y, z;
};

struct vec3u {
	unsigned x, y, z;
};

struct vec3 {
	float x, y, z;
};

struct dvec2 {
	double x, y;
};

struct vec2i {
	int x, y;
};

struct vec2u {
	unsigned x, y;
};

struct vec2 {
	float x, y;
};

float vec3_dot(const struct vec3 *a, const struct vec3 *b);

float vec3_norm(const struct vec3 *a);

struct vec3 vec3_sum(const struct vec3 *a, const struct vec3 *b);

struct vec3 vec3_sub(const struct vec3 *a, const struct vec3 *b);

struct vec3 vec3_cross(const struct vec3 *a, const struct vec3 *b);

struct vec3 vec3_normalize(const struct vec3 *a);

#endif
