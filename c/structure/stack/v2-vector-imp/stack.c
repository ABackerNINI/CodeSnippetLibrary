/** File: stack.c
 *  Tags: c,structure,stack
 *  Desc: This is a simple vector implmented stack.
 *
 *  2020/11/09
 *
 *  Compile with: gcc stack.c vector.c -W -Wall -Werror -o stack.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vector.h"

/*===========================================================================*/

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

/*===========================================================================*/

void stk_init(stack *s) { vec_init2(&s->vec, DEFAULT_STACK_CAPACITY); }

void stk_init2(stack *s, size_t capacity) { vec_init2(&s->vec, capacity); }

void stk_destory(stack *s) { vec_destory(&s->vec); }

void stk_push(stack *s, STACK_DATA_TYPE val) { vec_push_back(&s->vec, val); }

void stk_pop(stack *s) { vec_pop_back(&s->vec); }

STACK_DATA_TYPE stk_top(stack *s) { return vec_back(&s->vec); }

STACK_DATA_TYPE *stk_top_ref(stack *s) { return vec_back_ref(&s->vec); }

size_t stk_size(stack *s) { return vec_size(&s->vec); }

bool stk_empty(stack *s) { return stk_size(s) == 0; }

/*===========================================================================*/

int main() { return 0; }
