// Rodrigo Custodio
#include "bmp/bmp.h"
#include "gl/vector.h"
#include "models/models.h"

#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
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
	for(int i = 0; i < height; i++) {
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
	for(int i = 0; i < fbheight; i++) {
		for (int j = 0; j < fbwidth; j++) {
			fb[i][j] = 0;
		}
	}
}

void glClearColor(float r, float g, float b)
{
	int rint = floor(r >= 1.0 ? 255 : r * 255.0);
	int gint = floor(g >= 1.0 ? 255 : g * 255.0);
	int bint = floor(b >= 1.0 ? 255 : b * 255.0);
	int color = (rint << 16) + (gint << 8) + bint;
	for(int i = 0; i < fbheight; i++) {
		for (int j = 0; j < fbwidth; j++) {
			fb[i][j] = color;
		}
	}
}

static inline void point(int x, int y)
{
	if (x >= vpw || y >= vph) {
		return;
	}
	fb[y][x] = ccolor;
}

static int ndcToInt(float val, bool isXaxis)
{
	if (isXaxis) {
		int xw = ceil(vpx + (val + 1.0) * (vpw / 2.0));
		return (xw - 1) >= 0 ? xw - 1 : 0;
	} else {
		int yw = ceil(vpy + (val + 1.0) * (vph / 2.0));
		return (yw - 1) >= 0 ? yw - 1 : 0;
	}
}

void glVertex(float x, float y)
{
	int xw = ndcToInt(x, true);
	int yw = ndcToInt(y, false);
	point(xw, yw);
}

void glColor(float r, float g, float b)
{
	int rint = floor(r >= 1.0 ? 255 : r * 255.0);
	int gint = floor(g >= 1.0 ? 255 : g * 255.0);
	int bint = floor(b >= 1.0 ? 255 : b * 255.0);
	ccolor = (rint << 16) + (gint << 8) + bint;
}


void glLine(float x0, float y0, float x1, float y1)
{
	int x0w = ndcToInt(x0, true);
	int y0w = ndcToInt(y0, false);
	int x1w = ndcToInt(x1, true);
	int y1w = ndcToInt(y1, false);

	int dx = abs(x1w - x0w);
	int dy = abs(y1w - y0w);
	int steep = dy > dx;
	if (dy > dx) {
		x0w ^= y0w;
		y0w ^= x0w;
		x0w ^= y0w;
		x1w ^= y1w;
		y1w ^= x1w;
		x1w ^= y1w;
	}

	if (x0w > x1w) {
		x0w ^= x1w;
		x1w ^= x0w;
		x0w ^= x1w;

		y0w ^= y1w;
		y1w ^= y0w;
		y0w ^= y1w;
	}

	dy = abs(y1w - y0w);
	dx = abs(x1w - x0w);
	int y = y0w;
	int increment = (y0w > y1w) ? -1 : 1;
	int offset = 2 * dy - dx;
	for (int x = x0w; x < x1w; x++) {
		if (steep) {
			point(y, x);
		} else {
			point(x, y);
		}
		if (offset > 0) {
			y += increment;
			offset -= 2 *dx;
		}
		offset += 2 * dy;
	}
}

static inline float transform(float val, float trn, float scl)
{
	return (val + trn) * scl;
}

int glObj(char *filename, float trX, float trY, float scX, float scY)
{
	struct model *m = model_load(filename);
	if (m == NULL) {
		return -1;
	}
	for (size_t i = 0; i < m->faces->size; i++) {
		struct face *f = ds_vector_get(m->faces, i);
		for (size_t j = 0; j <  f->facedim; j++) {
			struct faced *from = ds_vector_get(f->data, j);
			struct faced *to = ds_vector_get(f->data,
							(j + 1) % f->facedim);
			struct vec3 *v1 = ds_vector_get(m->vertices,
							from->vi - 1);
			struct vec3 *v2 = ds_vector_get(m->vertices,
							to->vi - 1);
			glLine(transform(v1->x, trX, scX),
			       transform(v1->y, trY, scY),
			       transform(v2->x, trX, scX),
			       transform(v2->y, trY, scY));
		}
	}
	model_free(m);
	return 1;
}

static struct vec4 bounding(const float *ngon, size_t size)
{
	float minx = FLT_MAX;
	float maxx = -FLT_MAX;
	float miny = FLT_MAX;
	float maxy = -FLT_MAX;
	for (size_t i = 0; i < size; i += 2) {
		if (ngon[i] < minx) {
			minx = ngon[i];
		}

		if (ngon[i] > maxx) {
			maxx = ngon[i];
		}

		if (ngon[i + 1] <  miny) {
			miny = ngon[i + 1];
		}

		if (ngon[i + 1] > maxy) {
			maxy = ngon[i + 1];
		}
	}
	return (struct vec4) {
		.x = minx,
		.y = miny,
		.z = maxx,
		.w = maxy,
	};
}

void glNgon(const float *ngon, size_t size)
{
	if (size % 2 != 0) {
		return;
	}
	struct vec4 box = bounding(ngon, size);
	glLine(box.x, box.y, box.x, box.w);
	glLine(box.x, box.w, box.z, box.w);
	glLine(box.x, box.y, box.z, box.y);
	glLine(box.z, box.y, box.z, box.w);
	for(size_t i = 0; i < size; i += 2) {
		float x0 = ngon[i];
		float y0 = ngon[i + 1];
		float x1 = ngon[(i + 2) % size];
		float y1 = ngon[(i + 3) % size];
		glLine(x0, y0, x1, y1);
	}
}

void glFinish(void)
{
	bmp_write("canvas.bmp", fb, fbwidth, fbheight);
	for(size_t i = 0; i < vph; i++) {
		free(fb[i]);
	}
	free(fb);
}
