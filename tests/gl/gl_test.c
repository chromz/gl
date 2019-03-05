// Rodrigo Custodio

#include "gl/gl.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	glInit();
	glCreateWindow(600, 400);
	glViewport(100, 100, 100, 100);
	glClearColor(1.0, 1.0, 1.0);
	glClear();
	glColor(1.0, 0.0, 1.0);
	glVertex(0.0, 0.0);
	glVertex(-1.0, 1.0);
	glVertex(-1.0, -1.0);
	glVertex(1.0, -1.0);
	glVertex(1.0, 1.0);
	glVertex(1.5, 1.5);
	glVertex(-1.5, 1.5);
	glVertex(1.5, -1.5);
	glViewport(0, 0, 600, 400);
	glLine(-1.0, 1.0, 1.0, -1.0);
	glLine(0.3, 0.3, 0.3, 0.3);
	glLine(0.0, 0.0, 0.0, 1.0);
	glLine(-1.0, -1.0, 1.0, 1.0);
	glFinish();
	return 0;
}
