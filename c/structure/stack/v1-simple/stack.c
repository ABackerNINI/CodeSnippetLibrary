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
} stack_t;

void stk_init(stack_t *s);
void stk_init_by_capacity(stack_t *s, size_t capacity);
void stk_destory(stack_t *s);
void stk_push(stack_t *s, STACK_DATA_TYPE val);
void stk_pop(stack_t *s);
STACK_DATA_TYPE stk_top(stack_t *s);
STACK_DATA_TYPE *stk_top_ref(stack_t *s);
size_t stk_size(stack_t *s);
int stk_empty(stack_t *s);

/*===========================================================================*/

void stk_init(stack_t *s) { stk_init_by_capacity(s, DEFAULT_STACK_CAPACITY); }

void stk_init_by_capacity(stack_t *s, size_t capacity) {
    s->capacity = capacity;
    if (s->capacity > 0) {
        s->s = (STACK_DATA_TYPE *)malloc(sizeof(STACK_DATA_TYPE) * s->capacity);
    } else {
        s->s = NULL;
    }
    s->size = 0;
}

void stk_destory(stack_t *s) {
    if (s->s) free(s->s);
}

void stk_push(stack_t *s, STACK_DATA_TYPE val) {
    if (s->size == s->capacity) {
        s->capacity += s->capacity < 4 ? 1 : s->capacity / 2;
        s->s = (STACK_DATA_TYPE *)realloc(
            s->s, sizeof(STACK_DATA_TYPE) * s->capacity);
    }
    s->s[s->size++] = val;
}

void stk_pop(stack_t *s) { --s->size; }

STACK_DATA_TYPE stk_top(stack_t *s) { return s->s[s->size - 1]; }

STACK_DATA_TYPE *stk_top_ref(stack_t *s) { return &s->s[s->size - 1]; }

size_t stk_size(stack_t *s) { return s->size; }

int stk_empty(stack_t *s) { return s->size == 0; }

/*===========================================================================*/

int main() { return 0; }
