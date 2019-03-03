// Rodrigo Custodio

#define _DEFAULT_SOURCE
#include "gl/vector.h"
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
	if (c == EOF || c == '\n') {
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
	line[index] = '\0';
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

static void face_free(void *fptr)
{
	struct face *f = fptr;
	ds_vector_free(f->data);
	free(f);
}

static void parse_faces(struct ds_vector *faces, char *line)
{

	char *val = strsep(&line, " ");
	struct face *f = malloc(sizeof(struct face));
	f->data = ds_vector_new_with_free(elm_free);
	f->facedim = 0;
	while (val != NULL) {
		char  *res = strsep(&val, "/");
		struct faced *data = malloc(sizeof(struct faced));
		data->vi = strtoimax(res, NULL, 0);
		res = strsep(&val, "/");
		if (strcmp(res, "") == 0) {
			data->ti = -1;
		} else {
			data->ti = strtoimax(res, NULL, 0);
		}
		res = strsep(&val, "/");
		data->ni = strtoimax(res, NULL, 0);
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


struct model *model_load(char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return NULL;
	}
	struct model *mdl = malloc(sizeof(struct model));
	mdl->vertices = ds_vector_new_with_free(elm_free);
	mdl->faces = ds_vector_new_with_free(face_free);
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
		} else if (strcmp(pch, "vt") == 0) {
			//Textures
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
	return mdl;
}
