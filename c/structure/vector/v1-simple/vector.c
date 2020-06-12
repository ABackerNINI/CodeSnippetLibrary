#include <stdlib.h>
#include <string.h>

typedef struct {
    void *data;
    size_t capacity;
    size_t size;
} vec_t;

void vec_init(vec_t *v);
void vec_init2(vec_t *v, size_t capacity);
void vec_destroy(vec_t *v);
void vec_push_back(vec_t *v);
void vec_pop_back(vec_t *v);
int vec_empty();
size_t vec_size();
size_t vec_capacity();

void vec_init(vec_t *v) {
    v->data = NULL;
    v->capacity = 0;
    v->size = 0;
}

void vec_init2(vec_t *v, size_t capacity) {
    v->data = (void *)malloc(capacity);
}

void vec_destroy(vec_t *v) {}

void vec_push_back(vec_t *v) {}

void vec_pop_back(vec_t *v) {}

int vec_empty() {}

size_t vec_size() {}

size_t vec_capacity() {}

int main(int argc, char **argv) {
    printf("Hello world.\n");

    return 0;
}
