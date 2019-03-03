// Rodrigo Custodio

#include "gl/gl.h"
#include <stdio.h>


int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s <file>\n", argv[0]);
		printf("Output will always be on canvas.bmp\n");
		return 1;
	}
	glCreateWindow(800, 600);
	glColor(1.0, 1.0, 1.0);
	glObj(argv[1], 0.0, 0.0, .3, .3);
	glFinish();
	return 0;
}
