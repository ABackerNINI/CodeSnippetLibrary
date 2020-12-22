#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>
#include <stdbool.h>

#define VECTOR_DEBUG_LEVEL 1
#define DEFAULT_VECTOR_CAPACITY 64

typedef unsigned int VEC_DATA_TYPE; /* unsigned int should be enough */

typedef struct _vector {
    VEC_DATA_TYPE *data;
    size_t size;
    size_t capacity;
} vector_t;

void vec_init(vector_t *v);
void vec_init2(vector_t *v, size_t capacity);
void vec_init3(vector_t *v, size_t capacity, size_t size);
void vec_destory(vector_t *v);
void vec_push_back(vector_t *v, VEC_DATA_TYPE val);
void vec_pop_back(vector_t *v);
VEC_DATA_TYPE vec_front(vector_t *v);
VEC_DATA_TYPE *vec_front_ref(vector_t *v);
VEC_DATA_TYPE vec_back(vector_t *v);
VEC_DATA_TYPE *vec_back_ref(vector_t *v);
VEC_DATA_TYPE vec_at(vector_t *v, size_t index);
VEC_DATA_TYPE *vec_at_ref(vector_t *v, size_t index);
size_t vec_size(vector_t *v);
bool vec_empty(vector_t *v);
void vec_shrink(vector_t *v);

#endif  // __VECTOR_H__
