// Rodrigo Custodio

#define _DEFAULT_SOURCE
#include "models/models.h"
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



static char *readline(FILE *file)
{
	char *line = malloc(sizeof(char));
	int index = 0;
	int c = fgetc(file);
	if (c == EOF) {
		free(line);
		return NULL;
	}
	while (c != EOF && c != '\n') {
		line[index++] = c;
		char *l = realloc(line, (index + 1) * sizeof(char));
		if (l == NULL) {
			free(line);
			return NULL;
		}
		line = l;
		c = fgetc(file);
	}
	return line;
}


void model_free(struct model *m)
{
	ds_vector_free(m->vertices);
	ds_vector_free(m->faces);
	free(m);
}

static void elm_free(void *fptr)
{
	free(fptr);
}

static size_t parse_faces(struct ds_vector *faces, char *line)
{

	char *val = strsep(&line, " ");
	size_t facedim = 0;
	while (val != NULL) {
		char  *res = strsep(&val, "/");
		struct face *f = malloc(sizeof(struct face));
		f->vi = strtoimax(res, NULL, 0);
		res = strsep(&val, "/");
		if (strcmp(res, "") == 0) {
			f->ti = -1;
		} else {
			f->ti = strtoimax(res, NULL, 0);
		}
		res = strsep(&val, "/");
		f->ni = strtoimax(res, NULL, 0);
		if (errno) {
			free(f);
			return -1;
		}
		ds_vector_push_back(faces, f);
		val = strsep(&line, " ");
		facedim++;
	}
	return facedim;
}


struct model *model_load(char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return NULL;
	}
	struct model *mdl = malloc(sizeof(struct model));
	mdl->vertices = ds_vector_new_with_free(elm_free);
	mdl->faces = ds_vector_new_with_free(elm_free);
	mdl->facedim = 0;
	char *line;
	while ((line = readline(file)) != NULL) {
		char *tmp = line;
		char *pch = strsep(&tmp, " ");
		if (strcmp(pch, "v") == 0) {
			struct v3 *vertex = malloc(sizeof(struct v3));
			pch = strsep(&tmp, " ");
			vertex->x = strtod(pch, NULL);
			pch = strsep(&tmp, " ");
			vertex->y = strtod(pch, NULL);
			pch = strsep(&tmp, " ");
			vertex->z = strtod(pch, NULL);
			if (errno) {
				fclose(file);
				free(vertex);
				free(line);
				model_free(mdl);
				return NULL;
			}
			ds_vector_push_back(mdl->vertices, vertex);
		}else if (strcmp(pch, "vt") == 0) {
			//Textures
		} else if (strcmp(pch, "vn") == 0) {
			// Normals
		} else if (strcmp(pch, "f") == 0) {
			// Face
			size_t dim = parse_faces(mdl->faces, tmp);
			if (errno) {
				fclose(file);
				free(line);
				model_free(mdl);
				return NULL;
			}
			if (!mdl->facedim) {
				mdl->facedim = dim;
			}
		}
		free(line);
	}

	fclose(file);
	return mdl;
}
