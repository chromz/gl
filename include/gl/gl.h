// Rodrigo Custodio

#ifndef GL_H
#define GL_H
void glInit();

void glCreateWindow(int width, int height);

void glViewport(int x, int y, int width, int height);

void glClear();

void glClearColor(double r, double g, double b);

void glVertex(double x, double y);

void glColor(double r, double g, double b);

void glFinish();


#endif
