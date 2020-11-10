/** File: mempool.c
 *  Tags: c,structure,mempool
 *
 *  Desc: Use it when you need to allocate a lot of fixed size memory blocks.
 *      Compared with the v1-simple version, these memory blocks can be freed at
 *      any time. Compared with v2, there is no block headers. Memory efficient
 *      and reasonable time complexity for mempool_free().
 *
 *      mempool_alloc() has O(1) time complexit and mempool_free() has
 *      O(log2(n)) time complexity, where n is the number of big memory chunks
 *      (not the fixed size memory blocks).
 *
 *  Date: 2020/11/10
 *
 *  Compile with: gcc mempool.c stack.c vector.c -W -Wall -o mempool.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#include "stack.h"

/*===========================================================================*/

typedef struct {
    void *block;
    size_t ref;
    size_t block_no;
} mempool_block;

typedef struct {
    mempool_block **blocks;        /* data blocks, or data chunks is better? */
    mempool_block **sorted_blocks; /* blocks sorted by addr */
    size_t nblocks;                /* how many data blocks */
    size_t elem_size;              /* element size in bytes */
    size_t block_size;             /* how many elements a block can hold */
    size_t size;                   /* elements allocated */
    stack free_blocks;

    /* The next element position is (char*)blocks[_cur_block] + _cur_offset */

    size_t _max_offset; /* block size in bytes */
    size_t _cur_block;  /* current block */
    size_t _cur_offset; /* block offset */
} mempool;

void mpool_init(mempool *mp, size_t elem_size, size_t nelems_per_block);
void mpool_destory(mempool *mp);
void mpool_add_n_blocks(mempool *mp, size_t n);
void *mpool_alloc(mempool *mp);
void mpool_free(mempool *mp, void *ptr);
size_t mpool_size(mempool *mp);
size_t mpool_capacity(mempool *mp);
bool mpool_empty(mempool *mp);

static mempool_block *new_block(size_t size_in_bytes, size_t block_no);
static void free_block(mempool_block *block);
static int mpool_block_cmp(const void *b1, const void *b2);
static mempool_block *mpool_block_search(mempool_block **arr, size_t n,
                                         void *addr);

/*===========================================================================*/

/* Initialize the mempool. */
void mpool_init(mempool *mp, size_t elem_size, size_t nelems_per_block) {
    mp->blocks = NULL;
    mp->sorted_blocks = NULL;
    mp->nblocks = 0;
    mp->elem_size = elem_size;
    mp->block_size = nelems_per_block;
    mp->size = 0;
    stk_init(&mp->free_blocks);
    mp->_max_offset = elem_size * nelems_per_block;
    mp->_cur_block = 0;
    mp->_cur_offset = mp->_max_offset;
}

/* Destory the mempool. */
void mpool_destory(mempool *mp) {
    size_t i;
    for (i = 0; i < mp->nblocks; i++) {
        free_block(mp->blocks[i]);
    }
    free(mp->blocks);
    free(mp->sorted_blocks);
    stk_destory(&mp->free_blocks);
}

/* Add n memery blocks to the mempool, each block can hold nelems_per_block
 * elements. */
void mpool_add_n_blocks(mempool *mp, size_t n) {
    if (n == 0) return;
    size_t i, m = mp->nblocks + n;
    mp->blocks =
        (mempool_block **)realloc(mp->blocks, sizeof(mempool_block *) * (m));
    mp->sorted_blocks = (mempool_block **)realloc(
        mp->sorted_blocks, sizeof(mempool_block *) * (m));
    assert(mp->blocks);
    assert(mp->sorted_blocks);
    /* allocate n new blocks */
    for (i = mp->nblocks; i < m; i++) {
        mp->blocks[i] = new_block(mp->_max_offset, i);
        mp->sorted_blocks[i] = mp->blocks[i];
    }
    /* push these new blocks to free_blocks backward, so the first new block is
     * on top and will be used first */
    for (i = m - 1;; i--) {
        stk_push(&mp->free_blocks, i);
        if (i == mp->nblocks) break;
    }
    /* the array [0 - mp->nblocks-1] is previously sorted, and it's most likely
     * that each call of malloc() will return a larger address, so check if
     * blocks array is sorted before sorting */
    bool sorted = true;
    for (i = mp->nblocks ? mp->nblocks : 1; i < m; i++) {
        if (mp->sorted_blocks[i]->block < mp->sorted_blocks[i - 1]->block) {
            sorted = false;
            break;
        }
    }
    mp->nblocks += n;
    if (!sorted) { /* if not sorted, sort it */
        qsort(mp->sorted_blocks, mp->nblocks, sizeof(mempool_block *),
              mpool_block_cmp);
    }
}

/* Allocate memory for an element. */
void *mpool_alloc(mempool *mp) {
    if (mp->_cur_offset == mp->_max_offset) {
        /* if cur_offset reaches the max_offset, that means the current block is
         * used up, start using the next block */
        mp->_cur_offset = 0;
        if (stk_empty(&mp->free_blocks)) {
            /* all blocks are used up, add new blocks */
            mpool_add_n_blocks(mp, 1);
        }
        mp->_cur_block = stk_top(&mp->free_blocks);
        stk_pop(&mp->free_blocks);
    }
    mempool_block *block = mp->blocks[mp->_cur_block];
    void *addr = block->block + mp->_cur_offset;
    mp->_cur_offset += mp->elem_size; /* calc the next element pos */
    ++(block->ref);
    ++(mp->size);
    return addr; /* return the ptr */
}

/* Free memory of an element pointer returned by mpool_alloc(). */
void mpool_free(mempool *mp, void *ptr) {
    mempool_block *block =
        mpool_block_search(mp->sorted_blocks, mp->nblocks, ptr);
    assert(ptr >= block->block &&
           (size_t)(ptr - block->block) <= mp->_max_offset);
    assert(block->ref);
    if (--(block->ref) == 0) {
        if (mp->_cur_block == block->block_no) {
            /* if this is the currently using block, reset the offset */
            mp->_cur_offset = 0;
        } else {
            /* push to the free_blocks stack */
            stk_push(&mp->free_blocks, block->block_no);
        }
    }
}

/* Get number of elements allocated in the mempool. */
size_t mpool_size(mempool *mp) { return mp->size; }

/* Get number of elements the mempool can currently hold. */
size_t mpool_capacity(mempool *mp) { return mp->nblocks * mp->block_size; }

/* Check if no elements has been allocated in the mempool. */
bool mpool_empty(mempool *mp) { return mpool_size(mp) == 0; }

/*===========================================================================*/

/* Allocate a mempool_block. */
static mempool_block *new_block(size_t size_in_bytes, size_t block_no) {
    mempool_block *block = (mempool_block *)malloc(sizeof(mempool_block));
    assert(block);
    block->block = (void *)malloc(size_in_bytes);
    assert(block->block);
    block->ref = 0;
    block->block_no = block_no;
    return block;
}

/* Free a mempool block. */
static void free_block(mempool_block *block) {
    free(block->block);
    free(block);
}

/* Compare two mpool_blocks by their block address. */
static int mpool_block_cmp(const void *b1, const void *b2) {
    return (*(mempool_block **)b1)->block - (*(mempool_block **)b2)->block;
}

/* Perform binary search on the mempool_block array. Return the first block that
 * its block address <= addr. */
static mempool_block *mpool_block_search(mempool_block **arr, size_t n,
                                         void *addr) {
    assert(n);
    assert(addr >= arr[0]->block);
    size_t l = 0, r = n, mid;
    while (l + 1 < r) {
        mid = (l + r) >> 1;
        if (arr[mid]->block <= addr) {
            l = mid;
        } else {
            r = mid;
        }
    }
    return arr[l];
}

/*===========================================================================*/

void test_sorted() {
    mempool_block **arr = malloc(sizeof(mempool_block *) * 4);
    mempool_block *a = new_block(100, 0);
    mempool_block *b = new_block(100, 1);
    mempool_block *c = new_block(100, 2);
    mempool_block *d = new_block(100, 3);
    arr[0] = a;
    arr[1] = c;
    arr[2] = b;
    arr[3] = d;

    qsort(arr, 4, sizeof(mempool_block *), mpool_block_cmp);
    size_t i;
    for (i = 1; i < 4; i++) {
        assert(arr[i - 1] < arr[i]);
    }
    assert(mpool_block_search(arr, 4, a->block + 10) == a);
    assert(mpool_block_search(arr, 4, a->block) == a);
    assert(mpool_block_search(arr, 4, b->block + 10) == b);
    assert(mpool_block_search(arr, 4, b->block) == b);
    assert(mpool_block_search(arr, 4, c->block + 10) == c);
    assert(mpool_block_search(arr, 4, c->block) == c);
    assert(mpool_block_search(arr, 4, d->block + 10) == d);
    assert(mpool_block_search(arr, 4, d->block) == d);
    free_block(a);
    free_block(b);
    free_block(c);
    free_block(d);
    free(arr);
}

void random_test() {
    mempool mpool;
    mpool_init(&mpool, sizeof(int), 100);
    int i, N = rand() % 10000 + 1;
    int **arr = malloc(sizeof(int *) * N);
    memset(arr, 0, sizeof(int *) * N);
    for (i = 0; i < N; i++) {
        if (rand() % 2 == 0) {
            int *p = mpool_alloc(&mpool);
            *p = i;
            arr[i] = p;
        } else if (i) {
            int r = rand() % i;
            if (arr[r]) {
                mpool_free(&mpool, arr[r]);
                arr[r] = NULL;
            }
        }
    }

    for (i = 0; i < N; i++) {
        if (arr[i]) {
            assert(*arr[i] == i);
            mpool_free(&mpool, arr[i]);
        }
    }
    free(arr);
    mpool_destory(&mpool);
}

int main() {
    // int *a = malloc(sizeof(int));
    // int *b = malloc(sizeof(int));
    // int *c = malloc(sizeof(double));
    // int *d = malloc(sizeof(char));
    // int *e = malloc(sizeof(int));
    // int *f = malloc(sizeof(int));

    srand((unsigned int)time(NULL));

    test_sorted();

    const int N = 1000;
    int i;
    for (i = 0; i < N; i++) {
        random_test();
    }

    return 0;
}
