// Rodrigo Custodio
#include "bmp/bmp.h"
#include "gl/gl.h"
#include "models/models.h"

#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOLERANCE 0.00000001f

static int **fb;

static int fbwidth;
static int fbheight;
static int vpx = 0;
static int vpy = 0;
static int vpw = 0;
static int vph = 0;
static int ccolor = 0;
static struct vec3 *light = NULL;



void glInit(void)
{
	if (light == NULL) {
		light = malloc(sizeof(struct vec3));
		light->x = 0;
		light->y = 0;
		light->z = 0;
	}
}

static inline void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
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
	unsigned rint = floor(r >= 1.0 ? 255 : r * 255.0);
	unsigned gint = floor(g >= 1.0 ? 255 : g * 255.0);
	unsigned bint = floor(b >= 1.0 ? 255 : b * 255.0);
	int color = (rint << 16u) + (gint << 8u) + bint;
	for(int i = 0; i < fbheight; i++) {
		for (int j = 0; j < fbwidth; j++) {
			fb[i][j] = color;
		}
	}
}

static inline void point(int x, int y, int color)
{
	if (x >= vpw + vpx || y >= vph + vpy) {
		return;
	}
	fb[y][x] = color;
}

static int ndcToInt(float val, bool isXaxis)
{
	if (isXaxis) {
		int xw = (int) round(vpx + (val + 1.0) * (vpw / 2.0));
		return (xw - 1) >= 0 ? xw - 1 : 0;
	}

	int yw = (int) round(vpy + (val + 1.0) * (vph / 2.0));
	return (yw - 1) >= 0 ? yw - 1 : 0;
}

/* static float intToNdc(int val, bool isXaxis) */
/* { */
/* 	if (isXaxis) { */
/* 		return (val - vpx) * (2.0 / vpw) - 1.0; */
/* 	} else { */
/* 		return (val - vpy) * (2.0 / vph) - 1.0; */
/* 	} */
/* } */

void glVertex(float x, float y)
{
	int xw = ndcToInt(x, true);
	int yw = ndcToInt(y, false);
	point(xw, yw, ccolor);
}

static inline unsigned color24(unsigned r, unsigned g, unsigned b)
{
	return (r << 16u) + (g << 8u) + b;
}

void glColor(float r, float g, float b)
{
	unsigned rint = floor(r >= 1.0 ? 255 : r * 255.0);
	unsigned gint = floor(g >= 1.0 ? 255 : g * 255.0);
	unsigned bint = floor(b >= 1.0 ? 255 : b * 255.0);
	ccolor = color24(rint, gint, bint);
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
		swap(&x0w, &y0w);
		swap(&x1w, &y1w);
	}

	if (x0w > x1w) {
		swap(&x0w, &x1w);
		swap(&y0w, &y1w);
	}

	dy = abs(y1w - y0w);
	dx = abs(x1w - x0w);
	int y = y0w;
	int increment = (y0w > y1w) ? -1 : 1;
	int offset = 2 * dy - dx;
	for (int x = x0w; x < x1w; x++) {
		if (steep) {
			point(y, x, ccolor);
		} else {
			point(x, y, ccolor);
		}
		if (offset > 0) {
			y += increment;
			offset -= 2 *dx;
		}
		offset += 2 * dy;
	}
}

void glLight(float x, float y, float z)
{
	if (light == NULL) {
		light = malloc(sizeof(struct vec3));
	}
	light->x = x;
	light->y = y;
	light->z = z;
}

static struct vec4 bounding(const float *points, size_t size)
{
	float minx = FLT_MAX;
	float maxx = -FLT_MAX;
	float miny = FLT_MAX;
	float maxy = -FLT_MAX;
	for (size_t i = 0; i < size; i += 2) {
		if (points[i] < minx) {
			minx = points[i];
		}

		if (points[i] > maxx) {
			maxx = points[i];
		}

		if (points[i + 1] <  miny) {
			miny = points[i + 1];
		}

		if (points[i + 1] > maxy) {
			maxy = points[i + 1];
		}
	}
	return (struct vec4) {
		.x = minx,
		.y = miny,
		.z = maxx,
		.w = maxy,
	};
}

static inline float transform(float val, float trn, float scl)
{
	return (val + trn) * scl;
}

static inline struct vec3 vec3Transform(struct vec3 *v, const struct vec3 *trn,
					const struct vec3 *scl)
{
	return (struct vec3) {
		.x = transform(v->x, trn->x, scl->x),
		.y = transform(v->y, trn->y, scl->y),
		.z = transform(v->z, trn->z, scl->z),
	};
}

static void drawNgonFace(const struct model *m, const struct face *f,
			 const struct vec3 *trn,
			 const struct vec3 *scl)
{
	for (size_t j = 0; j <  f->facedim; j++) {
		struct facetup *from = ds_vector_get(f->data, j);
		struct facetup *to = ds_vector_get(f->data,
						(j + 1) % f->facedim);
		struct vec3 v1 = vec3Transform(ds_vector_get(m->vertices,
						from->vi - 1), trn, scl);
		struct vec3 v2 = vec3Transform(ds_vector_get(m->vertices,
						to->vi - 1), trn, scl);
		glLine(v1.x, v1.y, v2.x, v2.y);
	}
}

static void barycentric(const struct vec3 *a, const struct vec3 *b,
			const struct vec3 *c, const struct vec3 *p,
			float *w, float *v, float *u)
{
	float det = (b->y - c->y)*(a->x - c->x) + (c->x - b->x)*(a->y - c->y);
	if (det - 0.0f <= TOLERANCE ) {
		*u = -1.0;
		*v = -1.0;
		*w = -1.0;
		return;
	}
	*u = ((b->y - c->y)*(p->x - c->x) + (c->x - b->x)*(p->y - c->y)) / det;
	*v = ((c->y - a->y)*(p->x - c->x) + (a->x - c->x)*(p->y - c->y)) / det;
	*w = 1.0 - (*u + *v);
}

static void drawTriangle(const struct model *m, const struct face *f,
			 const struct vec3 *trn,
			 const struct vec3 *scl)
{
	struct facetup *af = ds_vector_get(f->data, 0);
	struct facetup *bf = ds_vector_get(f->data, 1);
	struct facetup *cf = ds_vector_get(f->data, 2);

	struct vec3 a = vec3Transform(ds_vector_get(m->vertices, af->vi - 1),
				       trn, scl);
	struct vec3 b = vec3Transform(ds_vector_get(m->vertices, bf->vi - 1),
				       trn, scl);
	struct vec3 c = vec3Transform(ds_vector_get(m->vertices, cf->vi - 1),
				       trn, scl);

	struct vec3 ab = vec3_sub(&b, &a);
	struct vec3 ac = vec3_sub(&c, &a);
	struct vec3 crs = vec3_cross(&ab, &ac);
	crs = vec3_normalize(&crs);

	float intensity = vec3_dot(&crs, light);
	int col = (int) round(255.0 * intensity);
	if (col < 0) {
		return;
	}
	col = color24(col, col, col);
	// Bounding box
	float minx = fminf(fminf(a.x, b.x), c.x);
	float miny = fminf(fminf(a.y, b.y), c.y);
	float maxx = fmaxf(fmaxf(a.x, b.x), c.x);
	float maxy = fmaxf(fmaxf(a.y, b.y), c.y);

	int minxi = ndcToInt(minx, true);
	int minyi = ndcToInt(miny, false);
	int maxxi = ndcToInt(maxx, true);
	int maxyi = ndcToInt(maxy, false);

	float dx = (maxx - minx) / (maxxi - minxi);
	float dy = (maxy - miny) / (maxyi - minyi);
	float y = miny;
	for (int yi = minyi; yi <= maxyi; yi++) {
		float x = minx;
		for (int xi = minxi; xi <= maxxi; xi++) {
			float w, v, u;
			struct vec3 p = {
				.x = x,
				.y = y,
				.z = 0,
			};
			barycentric(&a, &b, &c, &p, &w, &v, &u);
			if (w < 0.0f || v < 0.0f || u < 0.0f) {
				x += dx;
				continue;
			}
			point(xi, yi, col);
			x += dx;
		}
		y += dy;
	}
	// Just for testing the bounding box
	/* glLine(minx, miny, minx, maxy); */
	/* glLine(minx, maxy, maxx, maxy); */
	/* glLine(minx, miny, maxx, miny); */
	/* glLine(maxx, miny, maxx, maxy); */

}


int glObj(const char *filename, const struct vec3 *trn, const struct vec3 *scl)
{
	struct model *m = model_load(filename);
	if (m == NULL) {
		return -1;
	}
	// It's a triangle
	for (size_t i = 0; i < m->faces->size; i++) {
		struct face *f = ds_vector_get(m->faces, i);
		if (f->facedim == 3) {
			drawTriangle(m, f, trn, scl);
		} else {
			drawNgonFace(m, f, trn, scl);
		}
	}
	model_free(m);
	return 1;
}

/* static inline int getPoint(int x, int y) */
/* { */
/* 	if (x >= vpw && y < vph) { */
/* 		return fb[vpw -1][y]; */
/* 	} */
/* 	if (x < vpw && y >= vph) { */
/* 		return fb[x][vph - 1]; */
/* 	} */
/* 	return fb[y][x]; */
/* } */

static bool isInside(const float x, const float y,
		     const float *ngon, size_t size)
{
	// http://alienryderflex.com/polygon/
	bool odd = false;
	for (size_t i = 0; i < size; i += 2) {
		float x0 = ngon[i];
		float y0 = ngon[i + 1];
		float x1 = ngon[(i + 2) % size];
		float y1 = ngon[(i + 3) % size];
		if ((y0 < y && y1 >= y) ||
		    (y1 < y && y0 >= y)) {
			float intercept = x0 + (y - y0) / (y1 - y0) * (x1 - x0);
			odd ^= (unsigned) (intercept < x);
		}
	}
	return odd;
}

void glNgon(const float *ngon, size_t size)
{
	if (size % 2 != 0) {
		return;
	}
	struct vec4 box = bounding(ngon, size);

	int minx = ndcToInt(box.x, true);
	int miny = ndcToInt(box.y, false);
	int maxx = ndcToInt(box.z, true);
	int maxy = ndcToInt(box.w, false);
	float dx = (box.z - box.x) / (maxx - minx);
	float dy = (box.w - box.y) / (maxy - miny);
	float y = box.y;
	for (int yi = miny; yi < maxy; yi++) {
		float x = box.x;
		for (int xi = minx; xi < maxx; xi++) {
			if (isInside(x, y, ngon, size)) {
				point(xi, yi, ccolor);
			}
			x += dx;
		}
		y += dy;
	}

}

void glFinish(void)
{
	bmp_write("canvas.bmp", fb, fbwidth, fbheight);
	for(size_t i = 0; i < vph; i++) {
		free(fb[i]);
	}
	free(fb);
	free(light);
}
