// Rodrigo Custodio

#define _DEFAULT_SOURCE

#include <gl/gl.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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


static void draw_line(double start, double end)
{

}

static void square()
{

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
	}

	return 0;
}
