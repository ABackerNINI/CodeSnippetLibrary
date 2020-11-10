#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>
#include <stdbool.h>

#define VECTOR_DEBUG_LEVEL 1
#define DEFAULT_VECTOR_CAPACITY 64

typedef int VEC_DATA_TYPE;

typedef struct _vector {
    VEC_DATA_TYPE *data;
    size_t size;
    size_t capacity;
} vector;

void vec_init(vector *v);
void vec_init2(vector *v, size_t capacity);
void vec_init3(vector *v, size_t capacity, size_t size);
void vec_destory(vector *v);
void vec_push_back(vector *v, VEC_DATA_TYPE val);
void vec_pop_back(vector *v);
VEC_DATA_TYPE vec_front(vector *v);
VEC_DATA_TYPE *vec_front_ref(vector *v);
VEC_DATA_TYPE vec_back(vector *v);
VEC_DATA_TYPE *vec_back_ref(vector *v);
VEC_DATA_TYPE vec_at(vector *v, size_t index);
VEC_DATA_TYPE *vec_at_ref(vector *v, size_t index);
size_t vec_size(vector *v);
bool vec_empty(vector *v);
void vec_shrink(vector *v);

#endif // __VECTOR_H__
