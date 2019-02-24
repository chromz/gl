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

void glLine(double x0, double y0, double x1, double y1);

void glFinish();



#endif
