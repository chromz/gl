
#include "gl/gl.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	gl_init();
	gl_create_window(800, 800);
	gl_scale(1.0F, 1.0F, 1.0F);
	gl_light(0.0F, 0.0F, 1.0F);
	/* gl_look_at(0.0F, 1.0F, 3.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F); */
	gl_obj("labs/sphere.obj", NULL);
	/* gl_obj("/home/chromz/Downloads/model.obj", "/home/chromz/Downloads/model.bmp"); */
	gl_finish();
	return 0;
}
