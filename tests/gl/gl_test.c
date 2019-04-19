// Rodrigo Custodio

#include "gl/gl.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	gl_init();
	gl_create_window(600, 400);
	gl_viewport(100, 100, 100, 100);
	gl_clear_color(1.0, 1.0, 1.0);
	gl_clear();
	gl_color(1.0, 0.0, 1.0);
	gl_vertex(0.0, 0.0);
	gl_vertex(-1.0, 1.0);
	gl_vertex(-1.0, -1.0);
	gl_vertex(1.0, -1.0);
	gl_vertex(1.0, 1.0);
	/* gl_vertex(1.5, 1.5); */
	/* gl_vertex(-1.5, 1.5); */
	/* gl_vertex(1.5, -1.5); */
	/* gl_viewport(0, 0, 600, 400); */
	/* gl_line(-1.0, 1.0, 1.0, -1.0); */
	/* gl_line(0.3, 0.3, 0.3, 0.3); */
	/* gl_line(0.0, 0.0, 0.0, 1.0); */
	/* gl_line(-1.0, -1.0, 1.0, 1.0); */
	gl_finish();
	return 0;
}
