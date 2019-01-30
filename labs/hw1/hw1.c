// Rodrigo Custodio

#define _DEFAULT_SOURCE

#include <gl/gl.h>

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FEPSILON 0.00001



static void rpoint()
{
	glCreateWindow(800, 600);
	glColor(1.0, 1.0, 1.0);
	
	// Random points between [-1, 1]
	srandom(time(NULL));
	double x = (random() /  (float) RAND_MAX) * 2.0  - 1.0;
	double y = (random() /  (float) RAND_MAX) * 2.0  - 1.0;
	printf(" %f %f", x, y);
	glVertex(x, y);
	glFinish();
}

static void fourpoints()
{
	glCreateWindow(600, 400);
	glColor(1.0, 1.0, 1.0);
	glVertex(-1.0, 1.0);
	glVertex(-1.0, -1.0);
	glVertex(1.0, -1.0);
	glVertex(1.0, 1.0);
	glFinish();
}


static void draw_line(double startx, double starty, double endx, double endy)
{
	double delta = 0.001;
	if (fabs(endx - startx) <= FEPSILON && fabs(endy - starty) <= FEPSILON) {
		glVertex(startx, endx);
		return;
	}

	if (fabs(endx - startx) <= FEPSILON) {
		double slope = (endx - startx) / (endy - starty);
		if (endy - starty < 0) {
			delta *= -1.0;
		}
		for (double y = starty; fabs(endy - y) > FEPSILON ; y += delta) {
			glVertex(slope * (y - starty) + startx, y);
		}
	} else {
		double slope = (endy - starty) / (endx - startx);
		if (endx - startx < 0) {
			delta *= -1.0;
		}

		for (double x = startx; fabs(endx - x) > FEPSILON ; x += delta) {
			glVertex(x, slope * (x - startx) + starty);
		}
	}
	
}

static void square()
{
	glCreateWindow(600, 400);
	glColor(1.0, 1.0, 1.0);
	glVertex(0, 0);
	draw_line(-0.17, 0.25, 0.17, 0.25);
	draw_line(-0.17, 0.25, -0.17, -0.25);
	draw_line(0.17, 0.25, 0.17, -0.25);
	draw_line(-0.17, -0.25, 0.17, -0.25);
	glFinish();
}

static void diagonal()
{
	glCreateWindow(600, 400);
	glColor(1.0, 1.0, 1.0);
	draw_line(-1.0, -1.0, 1.0, 1.0);
	glFinish();
}

static void borders()
{
	glCreateWindow(600, 400);
	glColor(1.0, 1.0, 1.0);
	draw_line(-1.0, 1.0, 1.0, 1.0);
	draw_line(-1.0, 1.0, -1.0, -1.0);
	draw_line(-1.0, -1.0, 1.0, -1.0);
	draw_line(1.0, 1.0, 1.0, -1.0);
	glFinish();
}

static void gray_static()
{
	glCreateWindow(600, 400);
	srandom(time(NULL));
	for (double x = -1.0; x <= 1.0; x += 0.00333) {
		for (double y = -1.0; y <= 1.0; y += 0.00333) {
			double exp = (random() /  (float) RAND_MAX);
			if (exp <= 0.5) {
				glColor(1.0, 1.0, 1.0);
			} else {
				glColor(0.0, 0.0, 0.0);
			}
			glVertex(x, y);
		}
	}
	glFinish();
}

static void rgb_static()
{
	glCreateWindow(600, 400);
	srandom(time(NULL));
	for (double x = -1.0; x <= 1.0; x += 0.00333) {
		for (double y = -1.0; y <= 1.0; y += 0.00333) {
			double r = (random() /  (float) RAND_MAX);
			double g = (random() /  (float) RAND_MAX);
			double b = (random() /  (float) RAND_MAX);
			glColor(r, g, b);
			glVertex(x, y);
		}
	}
	glFinish();
}



int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s <option>\n", argv[0]);
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
	}

	return 0;
}
