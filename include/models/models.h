#ifndef MODELS_LIB_H
#define MODELS_LIB_H

#include <stdio.h>
#include <gmodule.h>

struct model {
	GArray *vertices;
	GArray *faces;
};

struct vertex {
	double x;
	double y;
	double z;
};

struct face_data {
	int vertex;
	int texture;
	int normal;
};

struct face {
	struct face_data *data;
	size_t size;
};


struct model load_model(char *filename);

#endif
