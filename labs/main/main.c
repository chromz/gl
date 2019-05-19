
#include "gl/gl.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define R_OFFSET 16U
#define G_OFFSET 8U
#define MAX_COL_VAL 255
#define MAX_COL_VAL_F 255.0F

static int width = 1920;
static int height = 1080;

static inline int color(unsigned r, unsigned g, unsigned b, float intensity)
{
	int col;
	col = (int) (((unsigned) (roundf(((float) r * intensity)))) <<
		      16U);
	col += (int) (((unsigned) (roundf((float) g * intensity))) <<
		       8U);
	col += (int) ((unsigned) (roundf((float) b * intensity)));
	return col;
}

static void set_texture_color(const struct model *m, float tx, float ty,
			      float intensity, int *col)
{
	int cx = (int) floorf(tx * (float) m->txwidth);
	int cy = (int) floorf(ty * (float) m->txheight);
	unsigned pcol = m->texture[cy * m->txwidth + cx];
	const unsigned MASK = 0xFFU;
	int tmp = (int) (pcol >> R_OFFSET & MASK);
	float greyscale = 0.2126 * tmp;
	tmp = (int) (pcol >> G_OFFSET & MASK);
	greyscale += 0.7152 * tmp;
	tmp = (int) (pcol & MASK);
	greyscale += 0.0722 * tmp;
	int grey = (int) roundf(greyscale);
	*col = color(grey, grey, grey, intensity);
}


static int greyscale_shader(const struct model *m, const struct vec3 *light,
			    const float u, const float v, const float w,
			    const int x, const int y, const struct vec3 *at,
			    const struct vec3 *bt, const struct vec3 *ct,
			    const struct vec3 *an, const struct vec3 *bn,
			    const struct vec3 *cn)
{
	float ia = vec3_dot(an, light);
	float ib = vec3_dot(bn, light);
	float ic = vec3_dot(cn, light);
	float intensity = ia * u + ib * v + ic * w;
	int col;
	if (at != NULL && bt != NULL && ct != NULL) {
		float tx = at->x * u + bt->x * v + ct->x * w;
		float ty = at->y * u + bt->y * v + ct->y * w;
		set_texture_color(m, tx, ty, intensity, &col);
	} else {
		col = (int) roundf(MAX_COL_VAL_F * intensity);
		return color(col, col, col, 1.0);
	}
	return col;
}

static void text_color(const struct model *m, float tx, float ty,
		       float intensity, int *col)
{
	float random_n = ((float)random() / (float) RAND_MAX);
	if (random_n < 0.4) {
		intensity *= 0.8F;
	}
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

static int noisy_shader(const struct model *m, const struct vec3 *light,
			   const float u, const float v, const float w,
			   const int x, const int y, const struct vec3 *at,
			   const struct vec3 *bt, const struct vec3 *ct,
			   const struct vec3 *an, const struct vec3 *bn,
			   const struct vec3 *cn)
{
	float ia = vec3_dot(an, light);
	float ib = vec3_dot(bn, light);
	float ic = vec3_dot(cn, light);
	float intensity = ia * u + ib * v + ic * w;
	int col;
	if (at != NULL && bt != NULL && ct != NULL) {
		float tx = at->x * u + bt->x * v + ct->x * w;
		float ty = at->y * u + bt->y * v + ct->y * w;
		text_color(m, tx, ty, intensity, &col);
	} else {
		col = (int) roundf(MAX_COL_VAL_F * intensity);
		return color(col, col, col, 1.0);
	}
	return col;
}

int main(int argc, char **argv)
{
	srandom(time(NULL));
	gl_init();
	gl_create_window(width, height);
	gl_look_at(0.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gl_background("labs/bg.bmp");
	gl_light(-1.0F, 0.0F, 1.0F);
	gl_rotate(1.0F, 0.0F, 1.0F, 0.0F);
	gl_scale(.5F, 1.0F, 0.1F);
	gl_translate(-3.3F, -1.9F, 0.0F);
	gl_shader(noisy_shader);
	gl_obj("labs/rex.obj", "labs/rex.bmp");
	gl_light(1.0F, 0.0F, 1.0F);
	gl_rotate(5.5F, 0.0F, 1.0F, 0.0F);
	gl_scale(.3F, .5F, .3F);
	gl_translate(1.3F, -1.88F, 0.0F);
	gl_obj("labs/ray.obj", "labs/ray.bmp");
	gl_light(-1.0F, 0.0F, 1.0F);
	gl_rotate(1.0F, 0.0F, 1.0F, 0.0F);
	gl_scale(.14F, .24F, .1F);
	gl_translate(-3.0F, -3.45F, 2.0F);
	gl_shader(greyscale_shader);
	gl_obj("labs/snake.obj", "labs/snake.bmp");
	gl_light(1.0F, 0.0F, -1.0F);
	gl_rotate(4.0F, 0.0F, 1.0F, 0.0F);
	gl_translate(3.80F, -4.6F, 3.0F);
	gl_scale(.1F, .18F, .1F);
	gl_obj("labs/nakedsnake.obj", "labs/nakedsnake.bmp");
	gl_light(1.0F, 0.5F, 1.0F);
	gl_scale(.05F, .1F, .05F);
	gl_rotate(-0.3F, 0.0, 1.0F, 0.0F);
	gl_translate(-20.0F, -7.5F, 5.0F);
	gl_obj("labs/box.obj", "labs/box.bmp");
	gl_finish();
	return 0;
}
