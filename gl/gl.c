// Rodrigo Custodio

#include "bmp/bmp.h"
#include "gl/gl.h"
#include "models/models.h"

#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOLERANCE 0.0000001F
#define MAX_COL_VAL 255
#define MAX_COL_VAL_F 255.0F
#define R_OFFSET 16U
#define G_OFFSET 8U

static int *fbuffer;
static float *zbuffer;
static float zdepth = 255.0F;

static int fbwidth;
static int fbheight;
static int vpx = 0;
static int vpy = 0;
static int vpw = 0;
static int vph = 0;
static int ccolor = 0;
static struct vec3 eye_vec;
static struct vec3 center_vec;
static struct vec3 *light = NULL;
static struct mat4f *scale_mat = NULL;
static struct mat4f *translation_mat = NULL;
static struct mat4f *viewport_mat = NULL;
static struct mat4f *look_at_mat = NULL;



void gl_init(void)
{
	if (light == NULL) {
		light = malloc(sizeof(*light));
		light->x = 0;
		light->y = 0;
		light->z = 0;
	}

	if (scale_mat == NULL) {
		scale_mat = mat4f_identity_p();
	}

	if (translation_mat == NULL) {
		translation_mat = mat4f_identity_p();
	}

	if (viewport_mat == NULL) {
		viewport_mat = mat4f_identity_p();
	}

	if (look_at_mat == NULL) {
		look_at_mat = mat4f_identity_p();
	}
}

static inline void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void gl_create_window(int width, int height)
{
	fbwidth = width;
	fbheight = height;
	fbuffer = calloc(width * height, sizeof(*fbuffer));
	zbuffer = malloc(sizeof(*zbuffer) * width * height);
	for (int i = 0; i < width * height; i++) {
		zbuffer[i] = -FLT_MAX;
	}
	vpw = width;
	vph = height;
}

void gl_viewport(int x, int y, int width, int height)
{
	mat4f_set(viewport_mat, 0, 3, (float) x + (float) width / 2.0F);
	mat4f_set(viewport_mat, 1, 3, (float) y + (float) height / 2.0F);
	mat4f_set(viewport_mat, 2, 3, zdepth / 2.0F);

	mat4f_set(viewport_mat, 0, 0, (float) width / 2.0F);
	mat4f_set(viewport_mat, 1, 1, (float) height / 2.0F);
	mat4f_set(viewport_mat, 2, 2, zdepth / 2.0F);
}

void gl_clear(void)
{
	for (int i = 0; i < fbwidth * fbheight; i++) {
		fbuffer[i] = 0;
	}
}

void gl_clear_color(float r, float g, float b)
{
	unsigned rint = (unsigned) floorf(r >= 1.0 ?
					  MAX_COL_VAL : r * MAX_COL_VAL_F);
	unsigned gint = (unsigned) floorf(g >= 1.0 ?
					  MAX_COL_VAL : g * MAX_COL_VAL_F);
	unsigned bint = (unsigned) floorf(b >= 1.0 ?
					  MAX_COL_VAL : b * MAX_COL_VAL_F);
	int color = (int) ((rint << R_OFFSET) + (gint << G_OFFSET) + bint);
	for (int i = 0; i < fbwidth * fbheight; i++) {
		fbuffer[i] = color;
	}
}

static inline void point(int x, int y, int color)
{
	if (x >= vpw + vpx || y >= vph + vpy || x >= fbwidth ||
	    y >= fbheight || x < 0 || y < 0) {
		return;
	}
	fbuffer[y * fbwidth + x] = color;
}

static inline void pointz(int x, int y, int color, float z)
{
	if (x >= vpw + vpx || y >= vph + vpy || x >= fbwidth ||
	    y >= fbheight || x < 0 || y < 0) {
		return;
	}
	int index = y * fbwidth + x;
	if (z > zbuffer[index]) {
		fbuffer[index] = color;
		zbuffer[index] = z;
	}

}

static int ndc_to_int(float val, bool isXaxis)
{
	if (isXaxis) {
		int xw = (int) roundf((float) vpx +
				      (val + 1.0F) * ((float) vpw / 2.0F));
		return (xw - 1) >= 0 ? xw - 1 : 0;
	}

	int yw = (int) roundf((float) vpy +
			      (val + 1.0F) * ((float) vph / 2.0F));
	return (yw - 1) >= 0 ? yw - 1 : 0;
}

void gl_vertex(float x, float y)
{
	int xw = ndc_to_int(x, true);
	int yw = ndc_to_int(y, false);
	point(xw, yw, ccolor);
}

static inline int color24(unsigned r, unsigned g, unsigned b)
{
	return (int) ((r << R_OFFSET) + (g << G_OFFSET) + b);
}

void gl_color(float r, float g, float b)
{
	unsigned rint = (unsigned) floorf(r >= 1.0F ?
					  MAX_COL_VAL : r * MAX_COL_VAL_F);
	unsigned gint = (unsigned) floorf(g >= 1.0F ?
					  MAX_COL_VAL : g * MAX_COL_VAL_F);
	unsigned bint = (unsigned) floorf(b >= 1.0F ?
					  MAX_COL_VAL : b * MAX_COL_VAL_F);
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
	assert(light != NULL);
	light->x = x;
	light->y = y;
	light->z = z;

	float norm = vec3_norm(light);
	light->x = x / norm;
	light->y = y / norm;
	light->z = z / norm;
}

void gl_look_at(struct vec3 eye, struct vec3 center, struct vec3 up)
{
	eye_vec = eye;
	center_vec = center;
	struct vec3 tmp = vec3_sub(&eye, &center);
	struct vec3 z = vec3_normalize(&tmp);
	tmp = vec3_cross(&up, &z);
	struct vec3 x = vec3_normalize(&tmp);
	tmp = vec3_cross(&z, &x);
	struct vec3 y = vec3_normalize(&tmp);
	struct mat4f tmp_mat = mat4f_identity();
	struct mat4f center_mat = mat4f_identity();

	mat4f_set(&tmp_mat, 0, 0, x.x);
	mat4f_set(&tmp_mat, 0, 1, x.y);
	mat4f_set(&tmp_mat, 0, 2, x.z);

	mat4f_set(&tmp_mat, 1, 0, y.x);
	mat4f_set(&tmp_mat, 1, 1, y.y);
	mat4f_set(&tmp_mat, 1, 2, y.z);

	mat4f_set(&tmp_mat, 2, 0, z.x);
	mat4f_set(&tmp_mat, 2, 1, z.y);
	mat4f_set(&tmp_mat, 2, 2, z.z);

	mat4f_set(&center_mat, 0, 3, -center.x);
	mat4f_set(&center_mat, 1, 3, -center.y);
	mat4f_set(&center_mat, 2, 3, -center.z);

	mat4f_mul4_set(look_at_mat, &tmp_mat, &center_mat);
}

static inline struct vec3 vec3_transform(struct vec3 *v)
{
	/* struct mat4f projection = mat4f_identity(); */
	struct vec4 aug;
	aug.x = v->x;
	aug.y = v->y;
	aug.z = v->z;
	aug.w = 1;
	struct vec4 tmp = mat4f_mul_vec4(scale_mat, &aug);
	// WIP
	tmp = mat4f_mul_vec4(translation_mat, &tmp);

	return (struct vec3) {
		.x = tmp.x / tmp.w,
		.y = tmp.y / tmp.w,
		.z = tmp.z / tmp.w,
	};
}


static void barycentric(const struct vec3 *a, const struct vec3 *b,
			const struct vec3 *c, const struct vec3 *p,
			float *w, float *v, float *u)
{
	float det = (b->y - c->y)*(a->x - c->x) + (c->x - b->x)*(a->y - c->y);
	if (det <= TOLERANCE ) {
		*u = -1.0F;
		*v = -1.0F;
		*w = -1.0F;
		return;
	}
	*u = ((b->y - c->y)*(p->x - c->x) + (c->x - b->x)*(p->y - c->y)) / det;
	*v = ((c->y - a->y)*(p->x - c->x) + (a->x - c->x)*(p->y - c->y)) / det;
	*w = 1.0F - (*u + *v);
}

static void set_texture_color(const struct model *m, float tx, float ty,
			      float intensity, int *col)
{
	int cx = (int) floorf(tx * (float) m->txwidth);
	int cy = (int) floorf(ty * (float) m->txheight);
	unsigned pcol = m->texture[cy * m->txwidth + cx];
	const unsigned MASK = 0xFFU;
	int tmp = (int) (pcol >> R_OFFSET & MASK);
	*col = (int) (((unsigned) (roundf(((float) tmp * intensity)))) <<
		      R_OFFSET);
	tmp = (int) (pcol >> G_OFFSET & MASK);
	*col += (int) (((unsigned) (roundf((float) tmp * intensity))) <<
		       G_OFFSET);
	tmp = (int) (pcol & MASK);
	*col += (int) ((unsigned) (roundf((float) tmp * intensity)));
}

static void draw_triangle(const struct model *m, const struct face *f)
{
	struct facetup *af = ds_vector_get(f->data, 0);
	struct facetup *bf = ds_vector_get(f->data, 1);
	struct facetup *cf = ds_vector_get(f->data, 2);

	struct vec3 a = vec3_transform(ds_vector_get(m->vertices, af->vi));
	struct vec3 b = vec3_transform(ds_vector_get(m->vertices, bf->vi));
	struct vec3 c = vec3_transform(ds_vector_get(m->vertices, cf->vi));
	

	struct vec3 ab = vec3_sub(&b, &a);
	struct vec3 ac = vec3_sub(&c, &a);
	struct vec3 crs = vec3_cross(&ab, &ac);
	crs = vec3_normalize(&crs);

	float intensity = vec3_dot(&crs, light);
	int col = 0;
	struct vec3 *at;
	struct vec3 *bt;
	struct vec3 *ct;
	if (!m->texture) {
		col = (int) roundf(MAX_COL_VAL_F * intensity);
		if (col < 0) {
			return;
		}
		col = color24(col, col, col);
	} else {
		at = ds_vector_get(m->textures, af->ti);
		bt = ds_vector_get(m->textures, bf->ti);
		ct = ds_vector_get(m->textures, cf->ti);
	}
	a.x = (float) ndc_to_int(a.x, true);
	a.y = (float) ndc_to_int(a.y, false);
	b.x = (float) ndc_to_int(b.x, true);
	b.y = (float) ndc_to_int(b.y, false);
	c.x = (float) ndc_to_int(c.x, true);
	c.y = (float) ndc_to_int(c.y, false);

	// Bounding box
	int minx = (int) fminf(fminf(a.x, b.x), c.x);
	int miny = (int) fminf(fminf(a.y, b.y), c.y);
	int maxx = (int) fmaxf(fmaxf(a.x, b.x), c.x);
	int maxy = (int) fmaxf(fmaxf(a.y, b.y), c.y);

	for (int y = miny; y <= maxy; y++) {
		for (int x = minx; x <= maxx; x++) {
			float w;
			float v;
			float u;
			struct vec3 p = {
				.x = (float) x,
				.y = (float) y,
				.z = 0,
			};
			barycentric(&a, &b, &c, &p, &w, &v, &u);
			if (w < 0.0F || v < 0.0F || u < 0.0F) {
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
		float x0 = (float) ndc_to_int(ngon[i], true);
		float y0 = (float) ndc_to_int(ngon[i + 1], false);
		float x1 = (float) ndc_to_int(ngon[(i + 2) % size], true);
		float y1 = (float) ndc_to_int(ngon[(i + 3) % size], false);
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
			if (is_inside((float) x, (float) y, ngon, size)) {
				point(x, y, ccolor);
			}
		}
	}
}

static float *setup_ngon(struct model *m, struct face *f)
{
	float *pol = malloc(2 * f->facedim * sizeof(*pol));
	size_t i = 0;
	size_t j = 0;
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
	int col = (int) roundf(MAX_COL_VAL_F * intensity);
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
			/* triangle_line_sweep(m, f); */
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

void gl_scale(float x, float y, float z)
{
	assert(scale_mat != NULL);
	scale_mat->data[0] = x;
	scale_mat->data[5] = y;
	scale_mat->data[10] = z;
}

void gl_translate(float x, float y, float z)
{
	assert(translation_mat != NULL);
	translation_mat->data[3] = x;
	translation_mat->data[7] = y;
	translation_mat->data[11] = z;
}

void gl_zbuffer(void)
{
	// Find the max and minimum
	float min = FLT_MAX;
	float max = -FLT_MAX;
	for (int y = 0; y < fbheight; y++) {
		for (int x = 0; x < fbwidth; x++) {
			float z = zbuffer[y * fbwidth + x];
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
			float z = zbuffer[y * fbwidth + x];
			if (fabsf(fabsf(z) - FLT_MAX) >= TOLERANCE) {
				int col = (int) roundf(MAX_COL_VAL_F *
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
	free(fbuffer);
	free(zbuffer);
	free(light);
	free(scale_mat);
	free(translation_mat);
	free(look_at_mat);
	free(viewport_mat);
}
