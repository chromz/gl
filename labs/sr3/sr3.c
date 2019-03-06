// Rodrigo Custodio

#include "gl/gl.h"
#include <stdio.h>


int main(void)
{
	glInit();
	glCreateWindow(800, 600);
	glColor(1.0, 1.0, 1.0);
	struct vec3 trn = {
		0.0,
		0.0,
		0.0,
	};
	struct vec3 scl = {
		1.0,
		1.0,
		1.0,
	};
	glLight(0.0, 0.0, 1.0);
	glObj("labs/test.obj", &trn, &scl);
	glFinish();
	return 0;
}
