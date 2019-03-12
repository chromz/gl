// Rodrigo Custodio

#include "gl/gl.h"
#include <stddef.h>
#include <stdio.h>

static void normalize_polygon(float *polygon, size_t size, 
			      int width, int height)
{
	for (size_t i = 0; i < size; i += 2) {
		polygon[i] = polygon[i] * (2.0 / (float) width) - 1.0;
		polygon[i + 1] = polygon[i + 1] * (2.0 / (float) height) - 1.0;
	}
}

int main(void)
{
	int width = 800;
	int height = 600;
	gl_init();
	gl_create_window(width, height);
	gl_color(1.0, 1.0, 1.0);

	float polygon1[20] = {
		165.0, 380.0,
		185.0, 360.0,
		180.0, 330.0,
		207.0, 345.0,
		233.0, 330.0,
		230.0, 360.0,
		250.0, 380.0,
		220.0, 385.0,
		205.0, 410.0,
		193.0, 383.0,
	};
	normalize_polygon(polygon1, 20, width, height);
	gl_ngon(polygon1, 20);

	gl_color(1.0, 0.0, 1.0);
	float polygon2[8] = {
		321.0, 335.0,
		288.0, 286.0,
		339.0, 251.0,
		374.0, 302.0,
	};
	normalize_polygon(polygon2, 8, width, height);
	gl_ngon(polygon2, 8);

	gl_color(1.0, 0.0, 0.0);
	float polygon3[6] = {
		377.0, 249.0,
		411.0, 197.0,
		436.0, 249.0,
	};
	normalize_polygon(polygon3, 6, width, height);
	gl_ngon(polygon3, 6);

	gl_color(0.0, 1.0, 1.0);
	float polygon4[36] = {
		413.0, 177.0,
		448.0, 159.0,
		502.0, 88.0,
		553.0, 53.0,
		535.0, 36.0,
		676.0, 37.0,
		660.0, 52.0,
		750.0, 145.0,
		761.0, 179.0,
		672.0, 192.0,
		659.0, 214.0,
		615.0, 214.0,
		632.0, 230.0,
		580.0, 230.0,
		597.0, 215.0,
		552.0, 214.0,
		517.0, 144.0,
		466.0, 180.0,
	};
	normalize_polygon(polygon4, 36, width, height);
	gl_ngon(polygon4, 36);

	gl_color(0.0, 0.0, 1.0);
	float polygon5[8] = {
		682.0, 175.0,
		708.0, 120.0,
		735.0, 148.0,
		739.0, 170.0,
	};
	normalize_polygon(polygon5, 8, width, height);
	gl_ngon(polygon5, 8);

	gl_finish();
	return 0;
}
