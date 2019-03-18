// Rodrigo Custodio

#include "gl/gl.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		return 1;
	}
	gl_init();
	gl_create_window(800, 600);
	gl_color(1.0, 1.0, 1.0);
	gl_translate(0.0, 0.0, 0.0);
	gl_scale(0.9, 0.9, 0.9);
	/* gl_scale(1.0, 1.0, 1.0); */
	gl_light(0.0, 0.0, 1.0);
	gl_obj(argv[1], NULL);
	gl_finish();
	return 0;
}
