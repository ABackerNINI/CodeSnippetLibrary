#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "vector.h"

void vec_init(vector *v) { vec_init2(v, DEFAULT_VECTOR_CAPACITY); }

void vec_init2(vector *v, size_t capacity) { vec_init3(v, capacity, 0); }

void vec_init3(vector *v, size_t capacity, size_t size) {
    v->capacity = capacity;
    if (v->capacity > 0) {
        v->data = (VEC_DATA_TYPE *)malloc(sizeof(VEC_DATA_TYPE) * v->capacity);
        assert(v->data);
    } else {
        v->data = NULL;
    }
    v->size = size;
}

void vec_destory(vector *v) {
    if (v->data) free(v->data);
}

void vec_push_back(vector *v, VEC_DATA_TYPE val) {
    if (v->size == v->capacity) {
        v->capacity += v->capacity < 4 ? 1 : v->capacity / 2;
        v->data = (VEC_DATA_TYPE *)realloc(v->data,
                                           sizeof(VEC_DATA_TYPE) * v->capacity);
        assert(v->data);
    }
    v->data[v->size++] = val;
}

void vec_pop_back(vector *v) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(v->size);
#endif
    v->size--;
}

VEC_DATA_TYPE vec_front(vector *v) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(v->size);
#endif
    return v->data[0];
}

VEC_DATA_TYPE *vec_front_ref(vector *v) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(v->size);
#endif
    return &v->data[0];
}

VEC_DATA_TYPE vec_back(vector *v) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(v->size);
#endif
    return v->data[v->size - 1];
}

VEC_DATA_TYPE *vec_back_ref(vector *v) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(v->size);
#endif
    return &v->data[v->size - 1];
}

VEC_DATA_TYPE vec_at(vector *v, size_t index) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(index < v->size);
#endif
    return v->data[index];
}

VEC_DATA_TYPE *vec_at_ref(vector *v, size_t index) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(index < v->size);
#endif
    return &v->data[index];
}

size_t vec_size(vector *v) { return v->size; }

bool vec_empty(vector *v) { return v->size == 0; }

void vec_shrink(vector *v) {
    if (v->size < v->capacity) {
        v->capacity = v->size;
        v->data = (VEC_DATA_TYPE *)realloc(v->data,
                                           sizeof(VEC_DATA_TYPE) * v->capacity);
        assert(v->data);
    }
}
