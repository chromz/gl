
#include "gl/gl.h"

int main(int argc, char **argv)
{
	if (argc != 3) {
		return 1;
	}
	gl_init();
	gl_create_window(800, 600);
	gl_color(1.0, 1.0, 1.0);
	gl_translate(0.0, 0.0, 0.0);
	gl_scale(0.8, 0.8, 0.8);
	/* gl_scale(1.0, 1.0, 1.0); */
	gl_light(1.0, -1.0, 1.0);
	gl_rotate(3.14/2.0, 0.0, 0.0, 0.0);
	gl_look_at(1.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gl_obj(argv[1], argv[2]);
	gl_finish();
	return 0;
}
