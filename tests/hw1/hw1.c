#include <stdio.h>
#include <gl/gl.h>

int main(int argc, char **argv) {
	glCreateWindow(800, 600);
	glClearColor(3, 4, 5);
	glFinish();
}
