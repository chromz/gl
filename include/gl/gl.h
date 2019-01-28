#ifndef HEADER_FILE
#define HEADER_FILE
void glInit();

void glCreateWindow(int width, int height);

void glViewPort(int x, int y, int width, int height);

void glClear();

void glClearColor(int r, int g, int b);

void glVertex(int x, int y);

void glColor(int r, int g, int b);

void glFinish();


#endif
