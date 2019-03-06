
#include "gl/gl.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		return 1;
	}
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
	glObj(argv[1], &trn, &scl);
	glFinish();
	return 0;
}
