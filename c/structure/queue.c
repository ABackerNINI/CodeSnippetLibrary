/** File: queue.c
 *  Tags: c,structure,queue
 *
 *  2019/9/13
 *
 *  Compile with: gcc queue.c -W -Wall -Werror -o queue.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===========================================================================*/

#define DEFAULT_QUEUE_CAPACITY 64

typedef int DATA_TYPE;

typedef struct _queue {
    DATA_TYPE *q;
    size_t l;
    size_t r;
    size_t capacity;
} queue;

void que_init(queue *q);
void que_init_by_capacity(queue *q, size_t capacity);
void que_destory(queue *q);
void que_push_back(queue *q, DATA_TYPE val);
void que_pop_front(queue *q);
DATA_TYPE que_front(queue *q);
DATA_TYPE *que_front_ref(queue *q);
size_t que_size(queue *q);
int que_empty(queue *q);

/*===========================================================================*/

void que_init(queue *q) { que_init_by_capacity(q, DEFAULT_QUEUE_CAPACITY); }

void que_init_by_capacity(queue *q, size_t capacity) {
    q->capacity = capacity;
    if (q->capacity > 0) {
        q->q = (DATA_TYPE *)malloc(sizeof(DATA_TYPE) * q->capacity);
    } else {
        q->q = NULL;
    }
    q->l = q->r = 0;
}

void que_destory(queue *q) {
    if (q->q) free(q->q);
}

void que_push_back(queue *q, DATA_TYPE val) {
    DATA_TYPE *new;
    size_t i, j, n, new_cap;
    if (que_size(q) == q->capacity) {
        new_cap = q->capacity + (q->capacity < 4 ? 1 : q->capacity / 2);
        new = (DATA_TYPE *)malloc(sizeof(DATA_TYPE) * new_cap);

        n = que_size(q);
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

    q->q[q->r] = val;
    q->r = (q->r + 1) % q->capacity;
}

void que_pop_front(queue *q) { q->l = (q->l + 1) % q->capacity; }

DATA_TYPE que_front(queue *q) { return q->q[q->l]; }

DATA_TYPE *que_front_ref(queue *q) { return &q->q[q->l]; }

size_t que_size(queue *q) {
    return q->l <= q->r ? q->r - q->l : q->r + q->capacity - q->l;
}

int que_empty(queue *q) { return que_size(q) == 0; }

/*===========================================================================*/

int main() { return 0; }
