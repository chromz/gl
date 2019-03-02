#ifndef MODELS_LIB_H
#define MODELS_LIB_H

#include "ds/vector.h"
#include <stdio.h>

struct model {
	struct ds_vector *vertices;
	struct ds_vector *faces;
};

struct vertex {
	double x;
	double y;
	double z;
};

struct face_data {
	size_t vi;
	size_t ti;
	size_t ni;
};

struct face {
	struct face_data *data;
	size_t size;
};


struct model *model_load(char *filename);

#endif
