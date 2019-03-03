#ifndef MODELS_LIB_H
#define MODELS_LIB_H

#include "gl/gl.h"
#include "ds/vector.h"
#include <stdio.h>

struct model {
	struct ds_vector *vertices;
	struct ds_vector *faces;
	size_t facedim;
};

struct face {
	int vi;
	int ti;
	int ni;
};


struct model *model_load(char *filename);

void model_free(struct model *m);

#endif
