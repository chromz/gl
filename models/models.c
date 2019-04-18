// Rodrigo Custodio

#define _DEFAULT_SOURCE
#include "gl/vector.h"
#include "bmp/bmp.h"
#include "models/models.h"
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static char *readline(FILE *file)
{
	char *line = malloc(sizeof(*line));
	int index = 0;
	int c = fgetc(file);
	if (c == EOF) {
		free(line);
		return NULL;
	}
	while (c != EOF && c != '\n') {
		line[index++] = c;
		char *l = realloc(line, (index + 1) * sizeof(*l));
		if (l == NULL) {
			free(line);
			return NULL;
		}
		line = l;
		c = fgetc(file);
	}
	line[index] = '\0';
	return line;
}


void model_free(struct model *m)
{
	free(m->texture);
	ds_vector_free(m->vertices);
	ds_vector_free(m->textures);
	ds_vector_free(m->faces);
	free(m);
}

static void elm_free(void *fptr)
{
	free(fptr);
}

static void face_free(void *fptr)
{
	struct face *f = fptr;
	ds_vector_free(f->data);
	free(f);
}

static void parse_faces(struct ds_vector *faces, char *line)
{

	char *val = strsep(&line, " ");
	struct face *f = malloc(sizeof(*f));
	f->data = ds_vector_new_with_free(elm_free);
	f->facedim = 0;
	while (val != NULL) {
		char  *res = strsep(&val, "/");
		struct facetup *data = malloc(sizeof(*data));
		data->vi = strtoimax(res, NULL, 0) - 1;
		res = strsep(&val, "/");
		if (strcmp(res, "") == 0) {
			data->ti = -1;
		} else {
			data->ti = strtoimax(res, NULL, 0) - 1;
		}
		res = strsep(&val, "/");
		data->ni = strtoimax(res, NULL, 0) - 1;
		if (errno) {
			free(data);
			face_free(f);
			return;
		}
		ds_vector_push_back(f->data, data);
		val = strsep(&line, " ");
		f->facedim++;
	}
	ds_vector_push_back(faces, f);
}

struct model *model_load(const char *filename, const char *txfilename)
{
	FILE *file = fopen(filename, "re");
	if (file == NULL) {
		return NULL;
	}
	struct model *mdl = malloc(sizeof(*mdl));
	mdl->textures = NULL;
	mdl->texture = NULL;
	mdl->txheight = 0;
	mdl->txwidth = 0;
	if (txfilename != NULL) {
		bmp_load(txfilename, &mdl->texture, &mdl->txwidth,
				&mdl->txheight);
	}
	mdl->vertices = ds_vector_new_with_free(elm_free);
	mdl->faces = ds_vector_new_with_free(face_free);
	mdl->textures = ds_vector_new_with_free(elm_free);
	char *line;
	while ((line = readline(file)) != NULL) {
		char *tmp;
		char *pch = strtok_r(line, " ", &tmp);
		if (!pch) {
			free(line);
			continue;
		}
		if (strcmp(pch, "v") == 0) {
			struct vec3 *vertex = malloc(sizeof(*vertex));
			pch = strtok_r(NULL, " ", &tmp);
			vertex->x = strtof(pch, NULL);
			pch = strtok_r(NULL, " ", &tmp);
			vertex->y = strtof(pch, NULL);
			pch = strtok_r(NULL, " ", &tmp);
			vertex->z = strtof(pch, NULL);
			if (errno) {
				fclose(file);
				free(vertex);
				free(line);
				model_free(mdl);
				return NULL;
			}
			ds_vector_push_back(mdl->vertices, vertex);
		} else if (strcmp(pch, "vt") == 0) {
			//Textures
			struct vec3 *tvertex = malloc(sizeof(*tvertex));
			pch = strtok_r(NULL, " ", &tmp);
			tvertex->x = strtof(pch, NULL);
			pch = strtok_r(NULL, " ", &tmp);
			tvertex->y = strtof(pch, NULL);
			pch = strsep(&tmp, " ");
			if (strcmp(pch, "") == 0) {
				tvertex->z = 0;
			} else {
				tvertex->z = strtof(pch, NULL);
			}
			if (errno) {
				fclose(file);
				free(tvertex);
				free(line);
				model_free(mdl);
				return NULL;
			}
			ds_vector_push_back(mdl->textures, tvertex);
		} else if (strcmp(pch, "vn") == 0) {
			// Normals
		} else if (strcmp(pch, "f") == 0) {
			// Face
			parse_faces(mdl->faces, tmp);

			if (errno) {
				fclose(file);
				free(line);
				model_free(mdl);
				return NULL;
			}
		}
		free(line);
	}

	fclose(file);
	ds_vector_shrink(mdl->vertices);
	ds_vector_shrink(mdl->faces);
	ds_vector_shrink(mdl->textures);
	return mdl;
}
