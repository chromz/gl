
#include <bmp/bmp.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FEPSILON 0.000001

static int **fb;

static int fbwidth;
static int fbheight;
static int vpx = 0;
static int vpy = 0;
static int vpw = 0;
static int vph = 0;
static int ccolor;


void glInit()
{
	// TODO
}

void glCreateWindow(int width, int height)
{
	fbwidth = width;
	fbheight = height;
	fb = malloc(sizeof(int*) * height);
	for(int i = 0; i < height; ++i) {
		fb[i] = calloc(width, sizeof(int));
	}
}

void glViewport(int x, int y, int width, int height)
{
	vpx= x;
	vpy = y;
	vpw = width;
	vph = height;
}

void glClear()
{
	for(int i = 0; i < fbheight; ++i) {
		memset(fb[i], 0, fbwidth * sizeof(int));
	}
}

void glClearColor(double r, double g, double b)
{
	int rint = floor(r >= 1.0 ? 255: r * 255.0);
	int gint = floor(g >= 1.0 ? 255: r * 255.0);
	int bint = floor(b >= 1.0 ? 255: r * 255.0);
	int color = (rint << 16) + (gint << 8) + bint;
	for(int i = 0; i < fbheight; ++i) {
		memset(fb[i], color, fbwidth * sizeof(int));
	}
}

void glVertex(double x, double y)
{
	if (fabs(x - 1.0) < FEPSILON || fabs(y - 1.0) < FEPSILON) {
		return;
	}
	int xw = vpx + (x + 1.0) * (vpw / 2.0);
	int yw = vpy + (y + 1.0) * (vph / 2.0);
	fb[yw][xw] = ccolor;
}

void glColor(int r, int g, int b)
{
	int rint = floor(r >= 1.0 ? 255: r * 255.0);
	int gint = floor(g >= 1.0 ? 255: r * 255.0);
	int bint = floor(b >= 1.0 ? 255: r * 255.0);
	ccolor = (rint << 16) + (gint << 8) + bint;
}

void glFinish()
{
	bmp_write(fb, fbwidth, fbheight, "canvas.bmp");
}
