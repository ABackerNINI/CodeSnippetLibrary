#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vector.h"

#define DEFAULT_STACK_CAPACITY 64

typedef VEC_DATA_TYPE STACK_DATA_TYPE;

typedef struct _stack {
    vector_t vec;
} stack_t;

void stk_init(stack_t *s);
void stk_init2(stack_t *s, size_t capacity);
void stk_destory(stack_t *s);
void stk_push(stack_t *s, STACK_DATA_TYPE val);
void stk_pop(stack_t *s);
STACK_DATA_TYPE stk_top(stack_t *s);
STACK_DATA_TYPE *stk_top_ref(stack_t *s);
size_t stk_size(stack_t *s);
bool stk_empty(stack_t *s);

#endif  // __STACK_H__
