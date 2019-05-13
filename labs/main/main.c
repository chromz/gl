
#include "gl/gl.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int width = 800;
int height = 800;

int main(int argc, char **argv)
{
	srandom(time(NULL));
	gl_init();
	gl_create_window(width, width);
	gl_background("/home/chromz/Pictures/pp.bmp");
	gl_light(0.0F, 0.0F, 1.0F);
	gl_look_at(0.5, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0);
	gl_scale(0.5F, 0.5F, 0.5F);
	gl_translate(0.0F, -1.0F, 1.0F);
	gl_obj("/home/chromz/Downloads/knight/knight2.obj", "/home/chromz/Downloads/knight/armor.bmp");
	gl_finish();
	return 0;
}
