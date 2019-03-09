// Rodrigo Custodio

#ifndef GL_H
#define GL_H

#include "gl/vector.h"
#include <stddef.h>

void glInit();

void glCreateWindow(int width, int height);

void glViewport(int x, int y, int width, int height);

void glClear();

void glClearColor(float r, float g, float b);

void glVertex(float x, float y);

void glColor(float r, float g, float b);

void glLine(float x0, float y0, float x1, float y1);

void glLight(float x, float y, float z);

int glObj(const char *filename, const char *txfilename);

void glNgon(const float *ngon, size_t size);

void glTranslate(float x, float y, float z);

void glScale(float x, float y, float z);

void glZBuffer(void);

void glFinish();



#endif
