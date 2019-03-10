
#include "gl/gl.h"
#include <string.h>

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("Usage <flat|zbuffer> <obj file>\n");
		return 1;
	}
	glInit();
	glCreateWindow(800, 600);
	glColor(1.0, 1.0, 1.0);
	glTranslate(0.0, 0.0, 0.0);
	glScale(4.5, 4.5, 4.5);
	glLight(0.0, 0.0, 1.0);
	if (strcmp(argv[1], "zbuffer") == 0) {
		glObj(argv[2], NULL);
		glZBuffer();
	} else if (strcmp(argv[1], "flat") == 0) {
		glObj(argv[2], NULL);
	}
	glFinish();
	return 0;
}
