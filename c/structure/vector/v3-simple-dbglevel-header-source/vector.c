#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

void vec_init(vector_t *v) { vec_init2(v, DEFAULT_VECTOR_CAPACITY); }

void vec_init2(vector_t *v, size_t capacity) { vec_init3(v, capacity, 0); }

void vec_init3(vector_t *v, size_t capacity, size_t size) {
    v->capacity = capacity;
    if (v->capacity > 0) {
        v->data = (VEC_DATA_TYPE *)malloc(sizeof(VEC_DATA_TYPE) * v->capacity);
        assert(v->data);
    } else {
        v->data = NULL;
    }
    v->size = size;
}

void vec_destory(vector_t *v) {
    if (v->data) free(v->data);
}

void vec_push_back(vector_t *v, VEC_DATA_TYPE val) {
    if (v->size == v->capacity) {
        v->capacity += v->capacity < 4 ? 1 : v->capacity / 2;
        v->data = (VEC_DATA_TYPE *)realloc(v->data,
                                           sizeof(VEC_DATA_TYPE) * v->capacity);
        assert(v->data);
    }
    v->data[v->size++] = val;
}

void vec_pop_back(vector_t *v) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(v->size);
#endif
    v->size--;
}

VEC_DATA_TYPE vec_front(vector_t *v) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(v->size);
#endif
    return v->data[0];
}

VEC_DATA_TYPE *vec_front_ref(vector_t *v) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(v->size);
#endif
    return &v->data[0];
}

VEC_DATA_TYPE vec_back(vector_t *v) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(v->size);
#endif
    return v->data[v->size - 1];
}

VEC_DATA_TYPE *vec_back_ref(vector_t *v) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(v->size);
#endif
    return &v->data[v->size - 1];
}

VEC_DATA_TYPE vec_at(vector_t *v, size_t index) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(index < v->size);
#endif
    return v->data[index];
}

VEC_DATA_TYPE *vec_at_ref(vector_t *v, size_t index) {
#if (VECTOR_DEBUG_LEVEL >= 1)
    assert(index < v->size);
#endif
    return &v->data[index];
}

size_t vec_size(vector_t *v) { return v->size; }

bool vec_empty(vector_t *v) { return v->size == 0; }

void vec_shrink(vector_t *v) {
    if (v->size < v->capacity) {
        v->capacity = v->size;
        v->data = (VEC_DATA_TYPE *)realloc(v->data,
                                           sizeof(VEC_DATA_TYPE) * v->capacity);
        assert(v->data);
    }
}
