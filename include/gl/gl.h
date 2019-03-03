// Rodrigo Custodio

#ifndef GL_H
#define GL_H

#include <stddef.h>

void glInit();

void glCreateWindow(int width, int height);

void glViewport(int x, int y, int width, int height);

void glClear();

void glClearColor(float r, float g, float b);

void glVertex(float x, float y);

void glColor(float r, float g, float b);

void glLine(float x0, float y0, float x1, float y1);

int glObj(char *filename, float trX, float trY, float scX, float scY);

void glNgon(const float *ngon, size_t size);

void glFinish();



#endif
