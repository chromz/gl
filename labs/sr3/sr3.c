// Rodrigo Custodio

#include "gl/gl.h"
#include <stdio.h>


int main(void)
{
	glCreateWindow(800, 600);
	glColor(1.0, 1.0, 1.0);
	glObj("labs/heli.obj", 0.0, 0.0, .09, .09);
	glFinish();
	return 0;
}
