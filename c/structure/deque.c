/** File: deque.c
 *  Tags: c,structure,deque
 *
 *  2019/9/13
 *
 *  Compile with: gcc deque.c -W -Wall -Werror -o deque.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===========================================================================*/

#define DEFAULT_DEQUE_CAPACITY 64

typedef int DATA_TYPE;

typedef struct _deque {
    DATA_TYPE *q;
    size_t l;
    size_t r;
    size_t capacity;
} deque;

void deque_init(deque *q);
void deque_init_by_capacity(deque *q, size_t capacity);
void deque_destory(deque *q);
void deque_push_front(deque *q, DATA_TYPE val);
void deque_push_back(deque *q, DATA_TYPE val);
void deque_pop_front(deque *q);
void deque_pop_back(deque *q);
DATA_TYPE deque_front(deque *q);
DATA_TYPE deque_back(deque *q);
DATA_TYPE *deque_front_ref(deque *q);
DATA_TYPE deque_back_ref(deque *q);
size_t deque_size(deque *q);
int deque_empty(deque *q);

void _deque_increase_capacity(deque *q);

/*===========================================================================*/

void deque_init(deque *q) { deque_init_by_capacity(q, DEFAULT_DEQUE_CAPACITY); }

void deque_init_by_capacity(deque *q, size_t capacity) {
    q->capacity = capacity;
    if (q->capacity > 0) {
        q->q = (DATA_TYPE *)malloc(sizeof(DATA_TYPE) * q->capacity);
    } else {
        q->q = NULL;
    }
    q->l = q->r = 0;
}

void deque_destory(deque *q) {
    if (q->q) free(q->q);
}

void deque_push_front(deque *q, DATA_TYPE val) {
    _deque_increase_capacity(q);

    q->l = (q->l == 0 ? q->capacity - 1 : q->l - 1);
    q->q[q->l] = val;
}

void deque_push_back(deque *q, DATA_TYPE val) {
    _deque_increase_capacity(q);

    q->q[q->r] = val;
    q->r = (q->r + 1) % q->capacity;
}

void deque_pop_front(deque *q) { q->l = (q->l + 1) % q->capacity; }

void deque_pop_back(deque *q) {
    q->r = (q->r == 0 ? q->capacity - 1 : q->r - 1);
}

DATA_TYPE deque_front(deque *q) { return q->q[q->l]; }

DATA_TYPE deque_back(deque *q) {
    return q->q[(q->r == 0 ? q->capacity - 1 : q->r - 1)];
}

DATA_TYPE *deque_front_ref(deque *q) { return &q->q[q->l]; }

DATA_TYPE deque_back_ref(deque *q) {
    return &q->q[(q->r == 0 ? q->capacity - 1 : q->r - 1)];
}

size_t deque_size(deque *q) {
    return q->l <= q->r ? q->r - q->l : q->r + q->capacity - q->l;
}

int deque_empty(deque *q) { return deque_size(q) == 0; }

void _deque_increase_capacity(deque *q) {
    DATA_TYPE *new;
    size_t i, j, n, new_cap;

    if (deque_size(q) == q->capacity) {
        new_cap = q->capacity + (q->capacity < 4 ? 1 : q->capacity / 2);
        new = (DATA_TYPE *)malloc(sizeof(DATA_TYPE) * new_cap);

        n = deque_size(q);
        // for (i = 0; i < n; ++i) {
        //     new[i] = q->q[(i + q->l) % q->capacity];
        // }
        if (q->l <= q->r) { /* [..l..r..] */

            for (i = 0; i < n; ++i) {
                new[i] = q->q[q->l + i];
            }
        } else { /* [..r..l..] */

            /* copy [l..] */
            j = q->capacity - q->l;
            for (i = 0; i < j; ++i) {
                new[i] = q->q[q->l + i];
            }
            /* copy [..r) */
            for (j = 0; j < q->r; ++i, ++j) {
                new[i] = q->q[j];
            }
        }
        if (q->q) free(q->q);
        q->q = new;
    }
}

/*===========================================================================*/

int main() { return 0; }
