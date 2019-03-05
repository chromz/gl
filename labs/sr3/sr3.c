// Rodrigo Custodio

#include "gl/gl.h"
#include <stdio.h>


int main(void)
{
	glCreateWindow(800, 600);
	glColor(1.0, 1.0, 1.0);
	struct vec3 trn = {
		0.0,
		0.0,
		0.0,
	};
	struct vec3 scl = {
		0.09,
		0.09,
		1.0,
	};
	glObj("labs/heli.obj", &trn, &scl);
	glFinish();
	return 0;
}
