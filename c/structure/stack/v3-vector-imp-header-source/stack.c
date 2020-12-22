#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stack.h"

void stk_init(stack_t *s) { vec_init2(&s->vec, DEFAULT_STACK_CAPACITY); }

void stk_init2(stack_t *s, size_t capacity) { vec_init2(&s->vec, capacity); }

void stk_destory(stack_t *s) { vec_destory(&s->vec); }

void stk_push(stack_t *s, STACK_DATA_TYPE val) { vec_push_back(&s->vec, val); }

void stk_pop(stack_t *s) { vec_pop_back(&s->vec); }

STACK_DATA_TYPE stk_top(stack_t *s) { return vec_back(&s->vec); }

STACK_DATA_TYPE *stk_top_ref(stack_t *s) { return vec_back_ref(&s->vec); }

size_t stk_size(stack_t *s) { return vec_size(&s->vec); }

bool stk_empty(stack_t *s) { return stk_size(s) == 0; }
