// Rodrigo Custodio

#define _DEFAULT_SOURCE

#include <gl/gl.h>

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FEPSILON 0.000001



static void rpoint(void)
{
	gl_create_window(800, 600);
	gl_color(1.0, 1.0, 1.0);
	
	// Random points between [-1, 1]
	srandom(time(NULL));
	float x = (random() /  (float) RAND_MAX) * 2.0  - 1.0;
	float y = (random() /  (float) RAND_MAX) * 2.0  - 1.0;
	gl_vertex(x, y);
	gl_finish();
}

static void fourpoints(void)
{
	gl_create_window(600, 400);
	gl_color(1.0, 1.0, 1.0);
	gl_vertex(-1.0, 1.0);
	gl_vertex(-1.0, -1.0);
	gl_vertex(1.0, -1.0);
	gl_vertex(1.0, 1.0);
	gl_finish();
}


static void draw_line(float startx, float starty, float endx, float endy)
{
	float delta = FEPSILON;
	if (fabs(endx - startx) <= FEPSILON &&
		fabs(endy - starty) <= FEPSILON) {
		gl_vertex(startx, endx);
		return;
	}

	if (fabs(endx - startx) <= FEPSILON) {
		float slope = (endx - startx) / (endy - starty);
		if (endy - starty < 0) {
			delta *= -1.0;
		}

		for (float y = starty; fabs(endy - y) > FEPSILON; y += delta) {
			gl_vertex(slope * (y - starty) + startx, y);
		}

	} else {
		float slope = (endy - starty) / (endx - startx);
		if (endx - startx < 0) {
			delta *= -1.0;
		}

		for (float x = startx; fabs(endx - x) > FEPSILON; x += delta) {
			gl_vertex(x, slope * (x - startx) + starty);
		}
	}
	
}


static void square(void)
{
	gl_create_window(600, 400);
	gl_color(1.0, 1.0, 1.0);
	gl_vertex(0, 0);
	draw_line(-0.17, 0.25, 0.17, 0.25);
	draw_line(-0.17, 0.25, -0.17, -0.25);
	draw_line(0.17, 0.25, 0.17, -0.25);
	draw_line(-0.17, -0.25, 0.17, -0.25);
	gl_finish();
}

static void diagonal(void)
{
	gl_create_window(600, 400);
	gl_color(1.0, 1.0, 1.0);
	draw_line(-1.0, -1.0, 1.0, 1.0);
	gl_finish();
}

static void borders(void)
{
	gl_create_window(600, 400);
	gl_color(1.0, 1.0, 1.0);
	draw_line(-1.0, 1.0, 1.0, 1.0);
	draw_line(-1.0, 1.0, -1.0, -1.0);
	draw_line(-1.0, -1.0, 1.0, -1.0);
	draw_line(1.0, 1.0, 1.0, -1.0);
	gl_finish();
}

static void gray_static(void)
{
	gl_create_window(600, 400);
	srandom(time(NULL));
	for (float x = -1.0; x <= 1.0; x += 0.00333) {
		for (float y = -1.0; y <= 1.0; y += 0.00333) {
			float exp = (random() /  (float) RAND_MAX);
			if (exp <= 0.5) {
				gl_color(1.0, 1.0, 1.0);
			} else {
				gl_color(0.0, 0.0, 0.0);
			}
			gl_vertex(x, y);
		}
	}
	gl_finish();
}

static void rgb_static(void)
{
	gl_create_window(600, 400);
	srandom(time(NULL));
	for (float x = -1.0; x <= 1.0; x += 0.00333) {
		for (float y = -1.0; y <= 1.0; y += 0.005) {
			float r = (random() /  (float) RAND_MAX);
			float g = (random() /  (float) RAND_MAX);
			float b = (random() /  (float) RAND_MAX);
			gl_color(r, g, b);
			gl_vertex(x, y);
		}
	}
	gl_finish();
}


static void fill(int width, int height)
{
	float dx = 2.0 / (width + 1);
	float dy = 2.0 / (height + 1);
	for (float x = -1.0; x <= 1.0; x += dx) {
		for (float y = -1.0; y <= 1.0; y += dy) {
			gl_vertex(x, y);
		}
	}

}

static void skyscrapper(float offset)
{
	draw_line(offset + 0.2, -0.32291666666, offset + 0.2875, -0.32291666666);
	draw_line(offset + 0.2, -0.31, offset + 0.2875, -0.31);
	draw_line(offset + 0.2, -0.30208333333, offset + 0.2875, -0.30208333333);
	draw_line(offset + 0.2, -0.29166666666, offset + 0.2875, -0.29166666666);
	draw_line(offset + 0.2, -0.28, offset + 0.2875, -0.28);
	draw_line(offset + 0.2, -0.27, offset + 0.2875, -0.27);
	draw_line(offset + 0.2, -0.26, offset + 0.2875, -0.26);
	draw_line(offset + 0.2, -0.25, offset + 0.2875, -0.25);
	draw_line(offset + 0.2, -0.24, offset + 0.2875, -0.24);
	draw_line(offset + 0.225, -0.23, offset + 0.2625, -0.23);
	draw_line(offset + 0.225, -0.22, offset + 0.2625, -0.22);
	draw_line(offset + 0.225, -0.21, offset + 0.2625, -0.21);
	draw_line(offset + 0.2375, -0.20, offset + 0.25, -0.20);
	draw_line(offset + 0.2375, -0.19, offset + 0.25, -0.19);
	draw_line(offset + 0.2375, -0.18, offset + 0.25, -0.18);
	draw_line(offset + 0.2375, -0.17, offset + 0.25, -0.17);
}

static void atari(void)
{
	gl_create_window(160, 192);
	gl_clear_color(0.0, 0.0, 0.0);
	gl_viewport(12, 66, 136, 125);
	gl_color(0.604, 0.784, 0);
	fill(136, 125);
	gl_viewport(0, 0, 160, 192);
	
	// Draw the bird
	gl_color(0.063, 0.212, 0);
	draw_line(-0.5625, 0.39583, -0.513, 0.39583);
	draw_line(-0.5375, 0.40625, -0.5375, 0.36458333333);
	draw_line(-0.5375, 0.3854, -0.4625, 0.3854);
	draw_line(-0.5375, 0.375, -0.4625, 0.375);
	draw_line(-0.5125, 0.36458333333, -0.4375, 0.36458333333);
	draw_line(-0.5125, 0.35416666666, -0.4125, 0.35416666666);
	draw_line(-0.5375, 0.34375, -0.5125, 0.34375);
	draw_line(-0.5625, 0.33333333333, -0.513, 0.33333333333);
	draw_line(-0.5625, 0.32291666666, -0.513, 0.32291666666);
	draw_line(-0.5375, 0.3125, -0.5125, 0.3125);
	draw_line(-0.4375, 0.34375, -0.3875, 0.34375);
	draw_line(-0.4375, 0.33333333333, -0.3875, 0.33333333333);
	
	// Shadow
	gl_color(0, 0.082, 0.004);
	draw_line(-0.55, 0.25, -0.5125, 0.25);
	draw_line(-0.5375, 0.23958333333, -0.4375, 0.23958333333);
	draw_line(-0.55, 0.22916666666, -0.5125, 0.22916666666);

	// Skyscraper
	gl_color(0.996, 0.875, 0.439);
	skyscrapper(0.0);
	skyscrapper(-0.40);
	skyscrapper(-0.80);

	// Zero
	draw_line(0.225, -0.375, 0.2625, -0.375);
	draw_line(0.225, -0.375, 0.2625, -0.375);
	draw_line(0.225, -0.42, 0.2625, -0.42);
	draw_line(0.225, -0.375, 0.225, -0.42);
	draw_line(0.2625, -0.375, 0.2625, -0.42);
	
	gl_finish();
}

void stars(void)
{
	gl_create_window(600, 400);
	srandom(time(NULL));
	gl_color(1.0, 1.0, 1.0);
	for (float x = -1.0; x <= 1.0; x += 0.00333) {
		for (float y = -1.0; y <= 1.0; y += 0.005) {
			float paint = (random() /  (float) RAND_MAX);
			if (paint < 0.001) {
				float big = (random() /  (float) RAND_MAX);
				if (big < 0.09) {
					gl_vertex(x, y);
					gl_vertex(x + 0.00333, y);
					gl_vertex(x - 0.00333, y);
					gl_vertex(x - 0.00333, y + 0.005);
					gl_vertex(x + 0.00333, y - 0.005);
					gl_vertex(x + 0.00333, y + 0.005);
					gl_vertex(x - 0.00333, y - 0.005);
					gl_vertex(x, y + 0.005);
					gl_vertex(x, y - 0.005);
				} else {
					gl_vertex(x, y);
				}
			}
		}
	}
	gl_finish();

}


int main(int argc, char **argv)
{
	gl_init();
	const char usage[] = "rpoint\n"
			     " Generate random point\n"
			     "fourpoints\n"
			     " Generate four points\n"
			     "square\n"
			     " Generate a 100x100 square\n"
			     "diagonal\n"
			     " Draw 1 line on each border\n"
			     "gray_static\n"
			     " Draw gray static\n"
			     "rgb_static\n"
			     " Draw rgb static\n"
			     "atari\n"
			     " Draw an ATARI 2600 screen\n"
			     "stars\n"
			     " Draw stars\n";
	if (argc != 2) {
		printf("Usage: %s <option>\n", argv[0]);
		printf("Output will always be on canvas.bmp\n");
		printf(usage);
		return 1;
	}

	if (strcmp("rpoint", argv[1]) == 0) {
		rpoint();
	} else if(strcmp("fourpoints", argv[1]) == 0) {
		fourpoints();
	} else if (strcmp("square", argv[1]) == 0) {
		square();
	} else if (strcmp("diagonal", argv[1]) == 0) {
		diagonal();
	} else if (strcmp("borders", argv[1]) == 0) {
		borders();
	} else if (strcmp("gray_static", argv[1]) == 0) {
		gray_static();
	} else if (strcmp("rgb_static", argv[1]) == 0) {
		rgb_static();
	} else if (strcmp("atari", argv[1]) == 0) {
		atari();
	} else if (strcmp("stars", argv[1]) == 0) {
		stars();
	} else {
		printf("Usage: %s <option>\n", argv[0]);
		printf("Output will always be on canvas.bmp\n");
		printf(usage);
	}

	return 0;
}
