// Rodrigo Custodio
#include <bmp/bmp.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static int **fb;

static int fbwidth;
static int fbheight;
static int vpx = 0;
static int vpy = 0;
static int vpw = 0;
static int vph = 0;
static int ccolor = 0;


void glInit(void)
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
	vpw = width;
	vph = height;
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
		for (int j = 0; j < fbwidth; ++j) {
			fb[i][j] = 0;
		}
	}
}

void glClearColor(double r, double g, double b)
{
	int rint = floor(r >= 1.0 ? 255 : r * 255.0);
	int gint = floor(g >= 1.0 ? 255 : g * 255.0);
	int bint = floor(b >= 1.0 ? 255 : b * 255.0);
	int color = (rint << 16) + (gint << 8) + bint;
	for(int i = 0; i < fbheight; ++i) {
		for (int j = 0; j < fbwidth; ++j) {
			fb[i][j] = color;
		}
	}
}

void glVertex(double x, double y)
{
	if (fabs(x) > 1.0 || fabs(y) > 1.0) {
		return;
	}
	int xw = ceil(vpx + (x + 1.0) * (vpw / 2.0));
	int yw = ceil(vpy + (y + 1.0) * (vph / 2.0));
	int inx = (xw - 1) >= 0 ? xw - 1 : 0;
	int iny = (yw - 1) >= 0 ? yw - 1 : 0;
	fb[iny][inx] = ccolor;
}

void glColor(double r, double g, double b)
{
	int rint = floor(r >= 1.0 ? 255 : r * 255.0);
	int gint = floor(g >= 1.0 ? 255 : g * 255.0);
	int bint = floor(b >= 1.0 ? 255 : b * 255.0);
	ccolor = (rint << 16) + (gint << 8) + bint;
}


void glLine(double x0, double y0, double x1, double y1)
{
	int x0w = ceil(vpx + (x0 + 1.0) * (vpw / 2.0));
	int y0w = ceil(vpy + (y0 + 1.0) * (vph / 2.0));
	int x1w = ceil(vpx + (x1 + 1.0) * (vpw / 2.0));
	int y1w = ceil(vpy + (y1 + 1.0) * (vph / 2.0));

	int dx = x1 - x0;
	int dy = y1 - y0;
	int yo = 1;
	if (dy < 0) {
		yo = -1;
		dy = -dy;
	}

}

void glFinish(void)
{
	bmp_write(fb, fbwidth, fbheight, "canvas.bmp");
}
