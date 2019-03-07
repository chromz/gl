// Rodrigo Custodio

#include "ds/vector.h"

#include <stdlib.h>

#define DS_VECTOR_INIT_SIZE 4


struct ds_vector *ds_vector_new(void)
{
	struct ds_vector *v = malloc(sizeof(struct ds_vector));
	v->data = malloc(DS_VECTOR_INIT_SIZE * sizeof(void *));
	v->size = 0;
	v->el_free = NULL;
	v->capacity = 4;
	return v;
}

struct ds_vector *ds_vector_new_with_free(void (*el_free)(void *))
{
	struct ds_vector *v = ds_vector_new();
	v->el_free = el_free;
	return v;
}

static bool resize(struct ds_vector *v, size_t min_cap)
{
	size_t old_cap = v->capacity;
	size_t new_cap = old_cap + (old_cap >> 1);
	if (new_cap == 0) {
		new_cap = min_cap;
	}
	void **new_data = realloc(v->data, sizeof(void *) * new_cap); 
	if (!new_data) {
		return false;
	}
	v->data = new_data;
	v->capacity = new_cap;
	return true;
}

bool ds_vector_push_back(struct ds_vector *v, void *e)
{
	if (v->capacity == v->size) {
		bool success = resize(v, true);
		if (!success) {
			return false;
		}
	}
	v->data[v->size++] = e;
	return true;
}

void *ds_vector_get(struct ds_vector *v, size_t i)
{
	if (i < v->size) {
		return v->data[i];
	}
	return NULL;
}

bool ds_vector_set(struct ds_vector *v, void *e, size_t i)
{
	if (i >= v->size) {
		return false;
	}
	v->data[i] = e;
	return true;
}

bool ds_vector_remove(struct ds_vector *v, size_t i)
{
	if (i >= v->size) {
		return false;
	}
	if (v->el_free) {
		v->el_free(v->data[i]);
	}
	for (size_t cpos = i; cpos < v->size - 1; cpos++) {
		v->data[cpos] = v->data[cpos + 1];
	}
	return true;
}

bool ds_vector_shrink(struct ds_vector *v)
{
	void **new_data = realloc(v->data, sizeof(void *) * v->size);
	if (new_data) {
		v->data = new_data;
		v->capacity = v->size;
		return true;
	}
	return false;
}

void ds_vector_free(struct ds_vector *v)
{
	if (v->el_free) {
		for (size_t i = 0; i < v->size; i++) {
			v->el_free(v->data[i]);
		}
	}
	free(v->data);
	free(v);
}

