
#include "gl/gl.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

int width = 800;
int height = 800;

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
	return color(47, 106, 140, intensity);
}

int main(int argc, char **argv)
{
	gl_init();
	gl_create_window(width, height);
	gl_scale(1.0F, 1.0F, 1.0F);
	gl_light(0.0F, 0.0F, 1.0F);
	/* gl_look_at(0.0F, 1.0F, 3.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F); */
	gl_shader(jupyter_shader);
	gl_obj("labs/sphere.obj", NULL);
	/* gl_obj("/home/chromz/Downloads/model.obj", "/home/chromz/Downloads/model.bmp"); */
	gl_finish();
	return 0;
}
