
#include "gl/gl.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	char *usage = "Usage:\n"
		      "medium: Medium shot render\n"
		      "dutch: Dutch angle\n"
		      "low: Low Angle\n"
		      "high: High angle\n";
	if (argc != 2) {
		puts(usage);
		return 1;
	}
	gl_init();
	gl_create_window(800, 600);
	gl_clear_color(0.819, 0.819, 0.819);
	/* gl_scale(1.8, 1.8, 1.8); */
	gl_scale(0.8F, 0.8F, 0.8F);
	gl_light(1.0, 0.0, 1.0);
	if (strcmp(argv[1], "medium") == 0){
		gl_look_at(0.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	} else if (strcmp(argv[1], "dutch") == 0){
		gl_look_at(1.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0);
	} else if (strcmp(argv[1], "low") == 0) {
		gl_look_at(0.0, -1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	} else if (strcmp(argv[1], "high") == 0) {
		gl_look_at(0.0, 2.5, 3.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0);
	} else {
		puts(usage);
		return 1;
	}
	gl_obj("labs/test.obj", "labs/test.bmp");
	gl_finish();
	return 0;
}
