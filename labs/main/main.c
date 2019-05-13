
#include "gl/gl.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


static const int width = 800;
static const int height = 800;
// Squared

static float focus_x = 430.0F;
static float focus_y = 400.0F;
static float grandient_w = 300.0F;


static float edistance(float x, float y, float x1, float y1)
{
	return sqrtf(powf(x1 - x, 2) + powf(y1 - y, 2));
}


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

static int interpolate_col(int r1, int g1,int b1, int r2, int g2,
			   int b2, float fraction, float intensity)
{
	int ri = (int)((float)(r1 - r2) * fraction + (float) r2);
	int gi = (int)((float)(g1 - g2) * fraction + (float) g2);
	int bi = (int)((float)(b1 - b2) * fraction + (float) b2);
	return  color(ri, gi, bi, intensity);
}


static int is_in_hurr(const int x, const int y, const int hurr_x,
		      const int hurr_y, const float scale_x,
		      const float scale_y, const float radius,
		      const float error)
{
	float xc = (x - width / 2.0F - hurr_x) * scale_x;
	float yc = (y - height / 2.0F - hurr_y) * scale_y;
	float a = 60.0F;
	if (edistance(0.0, 0.0, xc, yc) > radius) {
		return 0;
	}
	float tansq = tanf(sqrtf(xc * xc + yc *yc) / a);
	float res = xc * tansq;
	if (fabsf(yc - res) < error && yc < 460.0F && xc < 430.0F &&
	    xc > -520.0F && yc > -540.0F) {
		return 1;
	}
	return 0;
}


static int jupyter_shader(const struct model *m, const struct vec3 *light,
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
	if (intensity > 0.90F) intensity = 0.90F;
	if (intensity > 0.85F) intensity = 0.85F;
	else if (intensity > 0.60F) intensity = 0.80F;
	else if (intensity > 0.45F) intensity = 0.60F;
	else if (intensity > 0.30F) intensity = 0.45F;
	else if (intensity > 0.15F) intensity = 0.30F;
	else intensity = 0.2F;
	float d = edistance((float) x, (float) y, focus_x, focus_y);
	float factor = fminf(1.0F, d / grandient_w);
	int col = interpolate_col(171, 175, 176, 112, 102, 88, factor,
				  intensity);

	float random_n = ((float)random() /  (float) RAND_MAX);
	if (y > 550 && y < 650) {
		col = color(200, 163, 136, intensity);
		if (random_n < 0.01) {
			col = color(185, 192, 208, intensity);
		}
	}

	if (y > 540 && y < 551) {
		col = color(167, 174, 183, intensity);
		if (random_n < 0.01) {
			col = color(88, 75, 66, intensity);
		}
	}

	if (y > 510 && y < 541) {
		col = color(98, 62, 53, intensity);
		if (random_n < 0.01) {
			col = color(91, 99, 93, intensity);
		}
	}

	if (y > 420 && y < 511) {
		col = color(173, 174, 169, intensity);
		if (random_n < 0.01) {
			col = color(70, 46, 43, intensity);
		}
	}

	if (y > 350 && y < 421) {
		col = color(113, 91, 78, intensity);
		if (random_n < 0.01) {
			col = color(185, 192, 208, intensity);
		}
	}

	if (y > 300 && y < 351) {
		col = color(126, 130, 136, intensity);
		if (random_n < 0.01) {
			col = color(91, 99, 93, intensity);
		}
	}

	if (y > 290 && y < 301) {
		col = color(85, 81, 71, intensity);
		if (random_n < 0.01) {
			col = color(185, 192, 208, intensity);
		}
	}

	if (y > 260 && y < 291) {
		col = color(94, 83, 74, intensity);
		if (random_n < 0.01) {
			col = color(185, 192, 208, intensity);
		}
	}

	if (y > 230 && y < 261) {
		col = color(89, 81, 72, intensity);
		if (random_n < 0.01) {
			col = color(91, 99, 93, intensity);
		}
	}

	if (y < 231) {
		col = color(75, 62, 49, intensity);
		if (random_n < 0.01) {
			col = color(185, 192, 208, intensity);
		}
	}

	if (is_in_hurr(x, y, -80, -30, 10.5F, 10.5F, 450.0F, 655.0F)) {
		col = color(105, 46, 25, intensity);
	}

	if (is_in_hurr(x, y, 130, -70, 15.5F, 15.5F, 450.0F, 200.0F)) {
		col = color(135, 114, 87, intensity);
	}

	if (is_in_hurr(x, y, -20, -105, 25.5F, 25.5F, 450.0F, 700.0F)) {
		col = color(113, 109, 101, intensity);
	}

	if (is_in_hurr(x, y, -20, -105, 25.5F, 25.5F, 450.0F, 700.0F)) {
		col = color(113, 109, 101, intensity);
	}

	if (is_in_hurr(x, y, 70, -125, 35.5F, 35.5F, 450.0F, 700.0F)) {
		col = color(113, 109, 101, intensity);
	}

	if (is_in_hurr(x, y, 100, -125, 45.5F, 45.5F, 450.0F, 700.0F)) {
		col = color(113, 109, 101, intensity);
	}

	if (is_in_hurr(x, y, 130, -125, 55.5F, 55.5F, 450.0F, 700.0F)) {
		col = color(113, 109, 101, intensity);
	}

	return col;
}

static int moon_shader(const struct model *m, const struct vec3 *light,
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
	if (intensity > 0.90F) intensity = 0.90F;
	if (intensity > 0.85F) intensity = 0.85F;
	else if (intensity > 0.60F) intensity = 0.80F;
	else if (intensity > 0.45F) intensity = 0.60F;
	else if (intensity > 0.30F) intensity = 0.45F;
	else if (intensity > 0.15F) intensity = 0.30F;
	else intensity = 0.2F;
	float d = edistance((float) x, (float) y, focus_x, focus_y);
	float factor = fminf(1.0F, d / grandient_w);
	int col = interpolate_col(171, 175, 176, 112, 102, 88, factor,
				  intensity);
	return col;
}

int main(int argc, char **argv)
{
	srandom(time(NULL));
	gl_init();
	gl_create_window(width, height);
	gl_scale(1.0F, 1.0F, 1.0F);
	gl_light(0.3F, 0.4F, 1.0F);
	gl_shader(jupyter_shader);
	gl_obj("labs/sphere.obj", NULL);
	gl_shader(moon_shader);
	gl_scale(.1F, .1F, 1.0F);
	gl_translate(-5.8F, .5F, 1.0F);
	gl_obj("labs/sphere.obj", NULL);
	gl_scale(.15F, .15F, 1.0F);
	gl_translate(-1.8F, -4.5F, 1.0F);
	gl_obj("labs/sphere.obj", NULL);
	gl_scale(.08F, .08F, 1.0F);
	gl_translate(4.8F, -2.5F, 1.0F);
	gl_obj("labs/sphere.obj", NULL);
	gl_scale(.02F, .02F, 1.0F);
	gl_translate(5.8F, 30.5F, 1.0F);
	gl_obj("labs/sphere.obj", NULL);
	gl_finish();
	return 0;
}
