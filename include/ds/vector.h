
#ifndef DS_VECTOR_H
#define DS_VECTOR_H

#include <stddef.h>
#include <stdbool.h>

struct ds_vector {
	void **data;
	size_t size;
	size_t capacity;
	void (*el_free)(void *e);
};

struct ds_vector *ds_vector_new(void);

struct ds_vector *ds_vector_new_with_free(void (*el_free)(void *));

bool ds_vector_push_back(struct ds_vector *v, void *e);

void *ds_vector_get(struct ds_vector *v, size_t i);

bool ds_vector_set(struct ds_vector *v, void *e, size_t i);

bool ds_vector_remove(struct ds_vector *v, void *e, size_t i);

bool ds_vector_shrink(struct ds_vector *v);

void ds_vector_free(struct ds_vector *v);


#endif
