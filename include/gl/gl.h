// Rodrigo Custodio

#ifndef GL_H
#define GL_H

#include "gl/vector.h"
#include "gl/matrix.h"

#include <stddef.h>

void gl_init();

void gl_create_window(int width, int height);

void gl_viewport(int x, int y, int width, int height);

void gl_clear();

void gl_clear_color(float r, float g, float b);

void gl_vertex(float x, float y);

void gl_color(float r, float g, float b);

void gl_line(float x0, float y0, float x1, float y1);

void gl_light(float x, float y, float z);

void gl_look_at(float eyex, float eyey, float eyez, float centerx,
		float centery, float centerz, float upx, float upy, float upz);

int gl_obj(const char *filename, const char *txfilename);

void gl_ngon(const float *ngon, size_t size);

void gl_scale(float x, float y, float z);

void gl_translate(float x, float y, float z);

void gl_rotate(float angle, float x, float y, float z);

void gl_zbuffer(void);

void gl_finish();



#endif
