
#include "models/models.h"

struct model load_model(char *filename)
{
	struct model mdl = {
		.vertices = g_array_new(FALSE, FALSE, sizeof(struct model)),
		.faces = g_array_new(FALSE, FALSE, sizeof(struct face)),
	};

	return mdl;
}
