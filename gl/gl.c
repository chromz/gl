// Rodrigo Custodio
#include "bmp/bmp.h"
#include "gl/gl.h"
#include "models/models.h"

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOLERANCE 0.0000001f

static int **fbuffer;
static float **zbuffer;

static int fbwidth;
static int fbheight;
static int vpx = 0;
static int vpy = 0;
static int vpw = 0;
static int vph = 0;
static int ccolor = 0;
static struct vec3 *light = NULL;
static struct vec3 *trn = NULL;
static struct vec3 *scl = NULL;



void gl_init(void)
{
	if (light == NULL) {
		light = malloc(sizeof(struct vec3));
		light->x = 0;
		light->y = 0;
		light->z = 0;
	}
	if (trn == NULL) {
		trn = malloc(sizeof(struct vec3));
		trn->x = 0;
		trn->y = 0;
		trn->z = 0;
	}
	if (scl == NULL) {
		scl = malloc(sizeof(struct vec3));
		scl->x = 1.0;
		scl->y = 1.0;
		scl->z = 1.0;
	}
}

static inline void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

static inline void vecswap(struct vec3 *a, struct vec3* b)
{
	struct vec3 temp = *a;
	*a = *b;
	*b = temp;
}

void gl_create_window(int width, int height)
{
	fbwidth = width;
	fbheight = height;
	fbuffer = malloc(sizeof(int *) * height);
	zbuffer = malloc(sizeof(int *) * height);
	for(int i = 0; i < height; i++) {
		fbuffer[i] = calloc(width, sizeof(int));
		zbuffer[i] = malloc(width * sizeof(int));
		for (int j = 0; j < width; j++) {
			zbuffer[i][j] = -FLT_MAX;
		}
	}
	vpw = width;
	vph = height;
}

void gl_viewport(int x, int y, int width, int height)
{
	vpx= x;
	vpy = y;
	vpw = width;
	vph = height;
}

void gl_clear(void)
{
	for(int i = 0; i < fbheight; i++) {
		for (int j = 0; j < fbwidth; j++) {
			fbuffer[i][j] = 0;
		}
	}
}

void gl_clear_color(float r, float g, float b)
{
	unsigned rint = floor(r >= 1.0 ? 255 : r * 255.0);
	unsigned gint = floor(g >= 1.0 ? 255 : g * 255.0);
	unsigned bint = floor(b >= 1.0 ? 255 : b * 255.0);
	int color = (rint << 16u) + (gint << 8u) + bint;
	for(int i = 0; i < fbheight; i++) {
		for (int j = 0; j < fbwidth; j++) {
			fbuffer[i][j] = color;
		}
	}
}

static inline void point(int x, int y, int color)
{
	if (x >= vpw + vpx || y >= vph + vpy) {
		return;
	}
	fbuffer[y][x] = color;
}

static inline void pointz(int x, int y, int color, float z)
{
	if (x >= vpw + vpx || y >= vph + vpy) {
		return;
	}
	if (z > zbuffer[y][x]) {
		fbuffer[y][x] = color;
		zbuffer[y][x] = z;
	}

}

static int ndc_to_int(float val, bool isXaxis)
{
	if (isXaxis) {
		int xw = (int) roundf(vpx + (val + 1.0) * (vpw / 2.0));
		return (xw - 1) >= 0 ? xw - 1 : 0;
	}

	int yw = (int) roundf(vpy + (val + 1.0) * (vph / 2.0));
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

void gl_vertex(float x, float y)
{
	int xw = ndc_to_int(x, true);
	int yw = ndc_to_int(y, false);
	point(xw, yw, ccolor);
}

static inline unsigned color24(unsigned r, unsigned g, unsigned b)
{
	return (r << 16u) + (g << 8u) + b;
}

void gl_color(float r, float g, float b)
{
	unsigned rint = floor(r >= 1.0 ? 255 : r * 255.0);
	unsigned gint = floor(g >= 1.0 ? 255 : g * 255.0);
	unsigned bint = floor(b >= 1.0 ? 255 : b * 255.0);
	ccolor = color24(rint, gint, bint);
}

static void bresenham(int x0w, int y0w, int x1w, int y1w)
{
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

void gl_line(float x0, float y0, float x1, float y1)
{
	int x0w = ndc_to_int(x0, true);
	int y0w = ndc_to_int(y0, false);
	int x1w = ndc_to_int(x1, true);
	int y1w = ndc_to_int(y1, false);
	bresenham(x0w, y0w, x1w, y1w);
}

void gl_light(float x, float y, float z)
{
	if (light == NULL) {
		light = malloc(sizeof(struct vec3));
		light->x = 0.0;
		light->y = 0.0;
		light->z = 1.0;
	}
	float norm = sqrtf(powf(x, 2.0) + powf(y, 2.0) + powf(z, 2.0));
	light->x = x / norm;
	light->y = y / norm;
	light->z = z / norm;
}



static inline float transform(float val, float trn, float scl)
{
	return (val + trn) * scl;
}

static inline struct vec3 vec3_transform(struct vec3 *v, const struct vec3 *trn,
					const struct vec3 *scl)
{
	return (struct vec3) {
		.x = transform(v->x, trn->x, scl->x),
		.y = transform(v->y, trn->y, scl->y),
		.z = transform(v->z, trn->z, scl->z),
	};
}


static void draw_wireframe(const struct model *m, const struct face *f)
{
	for (size_t j = 0; j <  f->facedim; j++) {
		struct facetup *from = ds_vector_get(f->data, j);
		struct facetup *to = ds_vector_get(f->data,
						(j + 1) % f->facedim);
		struct vec3 v1 = vec3_transform(ds_vector_get(m->vertices,
						from->vi), trn, scl);
		struct vec3 v2 = vec3_transform(ds_vector_get(m->vertices,
						to->vi), trn, scl);
		gl_line(v1.x, v1.y, v2.x, v2.y);
	}
}

static void barycentric(const struct vec3 *a, const struct vec3 *b,
			const struct vec3 *c, const struct vec3 *p,
			float *w, float *v, float *u)
{
	float det = (b->y - c->y)*(a->x - c->x) + (c->x - b->x)*(a->y - c->y);
	if (det <= TOLERANCE ) {
		*u = -1.0;
		*v = -1.0;
		*w = -1.0;
		return;
	}
	*u = ((b->y - c->y)*(p->x - c->x) + (c->x - b->x)*(p->y - c->y)) / det;
	*v = ((c->y - a->y)*(p->x - c->x) + (a->x - c->x)*(p->y - c->y)) / det;
	*w = 1.0 - (*u + *v);
}

static void set_texture_color(const struct model *m, float tx, float ty,
			    float intensity, int *col)
{
	long cx = (long) floorf(tx * m->txwidth);
	long cy = (long) floorf(ty * m->txheight);
	unsigned pcol = m->texture[cy][cx];
	int tmp = pcol >> 16u & 0xFFu;
	*col = ((unsigned) (roundf((tmp * intensity)))) << 16u;
	tmp = pcol >> 8u & 0xFFu;
	*col += ((unsigned) (roundf(tmp * intensity))) << 8u;
	tmp = pcol & 0xFFu;
	*col += ((unsigned) (roundf(tmp * intensity)));
}

static void draw_triangle(const struct model *m, const struct face *f)
{
	struct facetup *af = ds_vector_get(f->data, 0);
	struct facetup *bf = ds_vector_get(f->data, 1);
	struct facetup *cf = ds_vector_get(f->data, 2);

	struct vec3 a = vec3_transform(ds_vector_get(m->vertices, af->vi),
				       trn, scl);
	struct vec3 b = vec3_transform(ds_vector_get(m->vertices, bf->vi),
				       trn, scl);
	struct vec3 c = vec3_transform(ds_vector_get(m->vertices, cf->vi),
				       trn, scl);
	

	struct vec3 ab = vec3_sub(&b, &a);
	struct vec3 ac = vec3_sub(&c, &a);
	struct vec3 crs = vec3_cross(&ab, &ac);
	crs = vec3_normalize(&crs);

	float intensity = vec3_dot(&crs, light);
	int col = 0;
	struct vec3 *at, *bt, *ct;
	if (!m->texture) {
		col = (int) roundf(255.0f * intensity);
		if (col < 0) {
			return;
		}
		col = color24(col, col, col);
	} else {
		at = ds_vector_get(m->textures, af->ti);
		bt = ds_vector_get(m->textures, bf->ti);
		ct = ds_vector_get(m->textures, cf->ti);
	}
	a.x = ndc_to_int(a.x, true);
	a.y = ndc_to_int(a.y, false);
	b.x = ndc_to_int(b.x, true);
	b.y = ndc_to_int(b.y, false);
	c.x = ndc_to_int(c.x, true);
	c.y = ndc_to_int(c.y, false);

	// Bounding box
	int minx = (int) fminf(fminf(a.x, b.x), c.x);
	int miny = (int) fminf(fminf(a.y, b.y), c.y);
	int maxx = (int) fmaxf(fmaxf(a.x, b.x), c.x);
	int maxy = (int) fmaxf(fmaxf(a.y, b.y), c.y);

	for (int y = miny; y <= maxy; y++) {
		for (int x = minx; x <= maxx; x++) {
			float w, v, u;
			struct vec3 p = {
				.x = x,
				.y = y,
				.z = 0,
			};
			barycentric(&a, &b, &c, &p, &w, &v, &u);
			if (w < 0.0f || v < 0.0f || u < 0.0f) {
				continue;
			}
			if (m->texture != NULL) {
				float tx = at->x * u + bt->x * v + ct->x * w;
				float ty = at->y * u + bt->y * v + ct->y * w;
				set_texture_color(m, tx, ty, intensity, &col);
				if (col < 0) {
					return;
				}
			}
			p.z = a.z * u + b.z * v + c.z * w;
			pointz(x, y, col, p.z);
		}
	}

	// Just for testing the bounding box
	/* gl_line(minx, miny, minx, maxy); */
	/* gl_line(minx, maxy, maxx, maxy); */
	/* gl_line(minx, miny, maxx, miny); */
	/* gl_line(maxx, miny, maxx, maxy); */

}

static void triangle_line_sweep(const struct model *m, const struct face *f)
{
	struct facetup *af = ds_vector_get(f->data, 0);
	struct facetup *bf = ds_vector_get(f->data, 1);
	struct facetup *cf = ds_vector_get(f->data, 2);

	struct vec3 a = vec3_transform(ds_vector_get(m->vertices, af->vi),
				       trn, scl);
	struct vec3 b = vec3_transform(ds_vector_get(m->vertices, bf->vi),
				       trn, scl);
	struct vec3 c = vec3_transform(ds_vector_get(m->vertices, cf->vi),
				       trn, scl);
	if (a.y > b.y) {
		vecswap(&a, &b);
	}

	if (a.y > c.y) {
		vecswap(&a, &c);
	}

	if (b.y > c.y) {
		vecswap(&b, &c);
	}

	struct vec3 ab = vec3_sub(&b, &a);
	struct vec3 ac = vec3_sub(&c, &a);
	struct vec3 crs = vec3_cross(&ab, &ac);
	crs = vec3_normalize(&crs);

	float intensity = vec3_dot(&crs, light);
	int col = (int) roundf(255.0f * intensity);
	if (col < 0) {
		return;
	}
	col = color24(col, col, col);

	a.x = ndc_to_int(a.x, true);
	a.y = ndc_to_int(a.y, false);
	b.x = ndc_to_int(b.x, true);
	b.y = ndc_to_int(b.y, false);
	c.x = ndc_to_int(c.x, true);
	c.y = ndc_to_int(c.y, false);
	int totalh = (int) (c.y - a.y);
	for (int y = 0; y <= totalh; y++) {
		
	}
	
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

static bool is_inside(const float x, const float y,
		     const float *ngon, size_t size)
{
	// http://alienryderflex.com/polygon/
	bool odd = false;
	for (size_t i = 0; i < size; i += 2) {
		float x0 = ndc_to_int(ngon[i], true);
		float y0 = ndc_to_int(ngon[i + 1], false);
		float x1 = ndc_to_int(ngon[(i + 2) % size], true);
		float y1 = ndc_to_int(ngon[(i + 3) % size], false);
		if ((y0 < y && y1 >= y) ||
		    (y1 < y && y0 >= y)) {
			float intercept = x0 + (y - y0) / (y1 - y0) * (x1 - x0);
			odd ^= (unsigned) (intercept < x);
		}
	}
	return odd;
}

void gl_ngon(const float *ngon, size_t size)
{
	if (size % 2 != 0) {
		return;
	}

	struct vec4 box = bounding(ngon, size);

	int minx = ndc_to_int(box.x, true);
	int miny = ndc_to_int(box.y, false);
	int maxx = ndc_to_int(box.z, true);
	int maxy = ndc_to_int(box.w, false);

	for (int y = miny; y <= maxy; y++) {
		for (int x = minx; x <= maxx; x++) {
			if (is_inside(x, y, ngon, size)) {
				point(x, y, ccolor);
			}
		}
	}
}

static float *setup_ngon(struct model *m, struct face *f)
{
	float *pol = malloc(2 * f->facedim * sizeof(float));
	size_t i, j = 0;
	struct facetup *fa = ds_vector_get(f->data, 0);
	struct vec3 *a = ds_vector_get(m->vertices, fa->vi);
	struct facetup *fb = ds_vector_get(f->data, 1);
	struct vec3 *b = ds_vector_get(m->vertices, fb->vi);
	struct facetup *fc = ds_vector_get(f->data, 2);
	struct vec3 *c = ds_vector_get(m->vertices, fc->vi);

	struct vec3 ab = vec3_sub(b, a);
	struct vec3 ac = vec3_sub(c, a);
	struct vec3 crs = vec3_cross(&ab, &ac);
	crs = vec3_normalize(&crs);

	float intensity = vec3_dot(&crs, light);
	int col = (int) roundf(255.0 * intensity);
	if (col < 0) {
		free(pol);
		return NULL;
	}
	ccolor = color24(col, col, col);
	
	for (i = 0; i < f->facedim; i++) {
		struct facetup *tup = ds_vector_get(f->data, i);
		struct vec3 *corner = ds_vector_get(m->vertices, tup->vi);
		pol[j] = corner->x;
		pol[j + 1] = corner->y;
		j += 2;
	}
	return pol;
}

int gl_obj(const char *filename, const char *txfilename)
{
	struct model *m = model_load(filename, txfilename);
	if (m == NULL) {
		return -1;
	}
	int tmp = ccolor;
	for (size_t i = 0; i < m->faces->size; i++) {
		struct face *f = ds_vector_get(m->faces, i);
		if (f->facedim == 3) {
			draw_triangle(m, f);
		} else if (f->facedim > 3) {
			// Experimental with all ngons
			float *vs = setup_ngon(m, f);
			if (vs != NULL) {
				gl_ngon(vs, f->facedim * 2);
				free(vs);
			}
			/* draw_wireframe(m, f); */
		}
	}
	ccolor = tmp;
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


void gl_translate(float x, float y, float z)
{
	if (trn == NULL) {
		trn = malloc(sizeof(struct vec3));
	}
	trn->x = x;
	trn->y = y;
	trn->z = z;
}

void gl_scale(float x, float y, float z)
{
	if (scl == NULL) {
		scl = malloc(sizeof(struct vec3));
	}
	scl->x = x;
	scl->y = y;
	scl->z = z;
}

void gl_zbuffer(void)
{
	// Find the max and minimum
	float min = FLT_MAX;
	float max = -FLT_MAX;
	for (int y = 0; y < fbheight; y++) {
		for (int x = 0; x < fbwidth; x++) {
			float z = zbuffer[y][x];
			if (fabsf((z + FLT_MAX)) >= TOLERANCE && z < min) {
				min = z;
			}
			if (fabsf((z - FLT_MAX)) >= TOLERANCE && z > max) {
				max = z;
			}
		}
	}

	for (int y = 0; y < fbheight; y++) {
		for (int x = 0; x < fbwidth; x++) {
			float z = zbuffer[y][x];
			if (fabsf(fabsf(z) - FLT_MAX) >= TOLERANCE) {
				int col = (int) roundf(255.0 *
						       (z - min) / (max - min));
				col = color24(col, col, col);
				point(x, y, col);
			}
		}
	}
}

void gl_finish(void)
{
	bmp_write("canvas.bmp", fbuffer, fbwidth, fbheight);
	for(size_t i = 0; i < vph; i++) {
		free(fbuffer[i]);
		free(zbuffer[i]);
	}
	free(fbuffer);
	free(zbuffer);
	free(light);
	free(trn);
	free(scl);
}
