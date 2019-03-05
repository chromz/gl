
#include "gl/gl.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		return 1;
	}
	glCreateWindow(800, 600);
	glColor(1.0, 1.0, 1.0);
	struct vec3 trn = {
		.8,
		-1.04,
		0.0,
	};
	struct vec3 scl = {
		4.0,
		4.0,
		1.0,
	};
	glObj(argv[1], &trn, &scl);
	glFinish();
	return 0;
}
