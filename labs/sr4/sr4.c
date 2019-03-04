
#include "gl/gl.h"

int main(void)
{
	glCreateWindow(800, 600);
	glColor(1.0, 1.0, 1.0);
	glObj("labs/heli.obj", -0.2, -0.2, .08, .08);
	glFinish();
	return 0;
}
