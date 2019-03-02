
#include "models/models.h"
#include <stdlib.h>
#include <stdio.h>



static char *getline(FILE *file)
{

}


struct model *model_load(char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return NULL;
	}
	struct model *mdl = malloc(sizeof(struct model));
	mdl->vertices = ds_vector_new();
	mdl->faces = ds_vector_new();
	/* while ((read = )) { */
	/* } */


	fclose(file);
	return mdl;
}
