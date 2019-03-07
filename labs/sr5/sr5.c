
#include "gl/gl.h"

int main(int argc, char **argv)
{
	glInit();
	glCreateWindow(800, 600);
	glColor(1.0, 1.0, 1.0);
	glTranslate(0.0, 0.0, 0.0);
	glScale(1.0, 1.0, 1.0);
	glLight(0.0, 0.0, 1.0);
	glObj("/home/chromz/Downloads/model.obj",
	      "/home/chromz/Downloads/model.bmp");
	glFinish();
	return 0;
}
