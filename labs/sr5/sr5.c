
#include "gl/gl.h"

int main(int argc, char **argv)
{
	if (argc != 3) {
		return 1;
	}
	glInit();
	glCreateWindow(800, 600);
	glColor(1.0, 1.0, 1.0);
	glTranslate(0.0, 0.0, 0.0);
	glScale(1.0, 1.0, 1.0);
	/* glScale(1.0, 1.0, 1.0); */
	glLight(1.0, 0.0, 1.0);
	glObj(argv[1], argv[2]);
	glFinish();
	return 0;
}
