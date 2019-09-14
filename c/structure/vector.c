/** File: vector.c
 *  Tags: c,structure,vector
 *
 *  2019/9/13
 *
 *  Compile with: gcc vector.c -W -Wall -Werror -o vector.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*===========================================================================*/

#define DEFAULT_VECTOR_CAPACITY 64

typedef int DATA_TYPE;

typedef struct _vector {
    DATA_TYPE *v;
    size_t size;
    size_t capacity;
} vector;

void vec_init(vector *v);
void vec_init_by_capacity(vector *v, size_t capacity);
void vec_init_by_capacity_size(vector *v, size_t capacity, size_t size);
void vec_destory(vector *v);
void vec_push_back(vector *v, DATA_TYPE val);
DATA_TYPE vec_back(vector *v);
DATA_TYPE *vec_back_ref(vector *v);
DATA_TYPE vec_at(vector *v, size_t index);
DATA_TYPE *vec_at_ref(vector *v, size_t index);
size_t vec_size(vector *v);
int vec_empty(vector *v);
void vec_shrink(vector *v);

/*===========================================================================*/

void vec_init(vector *v) { vec_init_by_capacity(v, DEFAULT_VECTOR_CAPACITY); }

void vec_init_by_capacity(vector *v, size_t capacity) {
    vec_init_by_capacity_size(v, capacity, 0);
}

void vec_init_by_capacity_size(vector *v, size_t capacity, size_t size) {
    v->capacity = capacity;
    if (v->capacity > 0) {
        v->v = (DATA_TYPE *)malloc(sizeof(DATA_TYPE) * v->capacity);
    } else {
        v->v = NULL;
    }
    v->size = size;
}

void vec_destory(vector *v) {
    if (v->v) free(v->v);
}

void vec_push_back(vector *v, DATA_TYPE val) {
    if (v->size == v->capacity) {
        v->capacity += v->capacity < 4 ? 1 : v->capacity / 2;
        v->v = (DATA_TYPE *)realloc(v->v, sizeof(DATA_TYPE) * v->capacity);
#if (DEBUG)
        printf("vector: realloc %ld @vec_push_back\n", v->capacity);
#endif
    }
    v->v[v->size++] = val;
}

DATA_TYPE vec_back(vector *v) { return v->v[v->size - 1]; }

DATA_TYPE *vec_back_ref(vector *v) { return &v->v[v->size - 1]; }

DATA_TYPE vec_at(vector *v, size_t index) {
    assert(index < v->size);
    return v->v[index];
}

DATA_TYPE *vec_at_ref(vector *v, size_t index) {
    assert(index < v->size);
    return &v->v[index];
}

size_t vec_size(vector *v) { return v->size; }

int vec_empty(vector *v) { return v->size == 0; }

void vec_shrink(vector *v) {
    if (v->size < v->capacity) {
        v->capacity = v->size;
        v->v = (DATA_TYPE *)realloc(v->v, sizeof(DATA_TYPE) * v->capacity);
    }
}

/*===========================================================================*/

void test_vec() {
    int i;
    vector v;
    vec_init_by_capacity_size(&v, 100, 10);

    for (i = 0; i < 10; ++i) {
        (*vec_at_ref(&v, i)) = i;
    }
    for (i = 0; i < 10; ++i) {
        printf("%d ", vec_at(&v, i));
    }
    printf("\n");

    for (i = 10; i < 2000; ++i) {
        vec_push_back(&v, i);
    }
    assert(vec_size(&v) == 2000);

    vec_destory(&v);
}

int main() {
    test_vec();

    return 0;
}
