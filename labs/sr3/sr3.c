// Rodrigo Custodio

#include "gl/gl.h"
#include <stdio.h>


int main(void)
{
	gl_init();
	gl_create_window(800, 600);
	gl_color(1.0, 1.0, 1.0);
	gl_light(0.0, 0.0, 1.0);
	gl_translate(0.0, 0.0, 0.0);
	gl_scale(1.0, 1.0, 1.0);
	gl_obj("labs/test.obj", NULL);
	gl_finish();
	return 0;
}
