/** File: stack.c
 *  Tags: c,structure,stack
 *
 *  2019/9/13
 *
 *  Compile with: gcc stack.c -W -Wall -Werror -o stack.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===========================================================================*/

#define DEFAULT_STACK_CAPACITY 64

typedef int STACK_DATA_TYPE;

typedef struct _stack {
    STACK_DATA_TYPE *s;
    size_t size;
    size_t capacity;
} stack;

void stk_init(stack *s);
void stk_init_by_capacity(stack *s, size_t capacity);
void stk_destory(stack *s);
void stk_push(stack *s, STACK_DATA_TYPE val);
void stk_pop(stack *s);
STACK_DATA_TYPE stk_top(stack *s);
STACK_DATA_TYPE *stk_top_ref(stack *s);
size_t stk_size(stack *s);
int stk_empty(stack *s);

/*===========================================================================*/

void stk_init(stack *s) { stk_init_by_capacity(s, DEFAULT_STACK_CAPACITY); }

void stk_init_by_capacity(stack *s, size_t capacity) {
    s->capacity = capacity;
    if (s->capacity > 0) {
        s->s = (STACK_DATA_TYPE *)malloc(sizeof(STACK_DATA_TYPE) * s->capacity);
    } else {
        s->s = NULL;
    }
    s->size = 0;
}

void stk_destory(stack *s) {
    if (s->s) free(s->s);
}

void stk_push(stack *s, STACK_DATA_TYPE val) {
    if (s->size == s->capacity) {
        s->capacity += s->capacity < 4 ? 1 : s->capacity / 2;
        s->s = (STACK_DATA_TYPE *)realloc(s->s, sizeof(STACK_DATA_TYPE) * s->capacity);
    }
    s->s[s->size++] = val;
}

void stk_pop(stack *s) { --s->size; }

STACK_DATA_TYPE stk_top(stack *s) { return s->s[s->size - 1]; }

STACK_DATA_TYPE *stk_top_ref(stack *s) { return &s->s[s->size - 1]; }

size_t stk_size(stack *s) { return s->size; }

int stk_empty(stack *s) { return s->size == 0; }

/*===========================================================================*/

int main() { return 0; }
