#include <stdio.h>
#include <gl/gl.h>

int main(int argc, char **argv) {
	glCreateWindow(600, 400);
	glClearColor(1.0, 1.0, 1.0);
	glClear();
	glColor(1.0, 0.0, 1.0);
	glVertex(0.0, 0.0);
	glFinish();
}
