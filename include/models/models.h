#ifndef MODELS_LIB_H
#define MODELS_LIB_H

#include "ds/vector.h"
#include <stdio.h>

struct model {
	struct ds_vector *vertices;
	struct ds_vector *faces;
	struct ds_vector *textures;
	int *texture;
	long txwidth;
	long txheight;
};

struct facetup {
	int vi;
	int ti;
	int ni;
};

struct face {
	struct ds_vector *data;
	size_t facedim;
};


struct model *model_load(const char *filename, const char *txfilename);

void model_free(struct model *m);

#endif
