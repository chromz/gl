
#include "gl/gl.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		return 1;
	}
	glCreateWindow(800, 600);
	glColor(1.0, 1.0, 1.0);
	glObj(argv[1], 3.2, -4.0, 4.0, 4.0);
	glFinish();
	return 0;
}
