#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vector.h"

#define DEFAULT_STACK_CAPACITY 64

typedef VEC_DATA_TYPE STACK_DATA_TYPE;

typedef struct _stack {
    vector vec;
} stack;

void stk_init(stack *s);
void stk_init2(stack *s, size_t capacity);
void stk_destory(stack *s);
void stk_push(stack *s, STACK_DATA_TYPE val);
void stk_pop(stack *s);
STACK_DATA_TYPE stk_top(stack *s);
STACK_DATA_TYPE *stk_top_ref(stack *s);
size_t stk_size(stack *s);
bool stk_empty(stack *s);

#endif  // __STACK_H__
