/** File: mempool.c
 *  Tags: c,structure,mempool
 *
 *  Desc: Use it when you need to allocate a lot of fixed size memory blocks,
 *      and thoes memory blocks are not going to be freed until the end.
 *
 *      mempool_alloc() has O(1) time complexity.
 *
 *  Date: 2020/11/8
 *
 *  Compile with: gcc mempool.c -W -Wall -o mempool.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

/*===========================================================================*/

typedef struct {
    void **blocks;     /* data blocks */
    size_t nblocks;    /* how many data blocks */
    size_t elem_size;  /* element size in bytes */
    size_t block_size; /* how many elements a block can hold */

    /* Can use 'size' to represent all the following three members, but to avoid
     * repeated calculation, use them */
    /* The next element position is (char*)blocks[_cur_block] + _cur_offset */

    size_t _max_offset; /* block size in bytes */
    size_t _cur_block;  /* current block */
    size_t _cur_offset; /* block offset */
} mempool;

void mpool_init(mempool *mp, size_t elem_size, size_t nelems_per_block);
void mpool_destory(mempool *mp);
void mpool_add_n_blocks(mempool *mp, size_t n);
void *mpool_alloc(mempool *mp);
void *mpool_at(mempool *mp, size_t index);
size_t mpool_size(mempool *mp);
size_t mpool_capacity(mempool *mp);
bool mpool_empty(mempool *mp);

/*===========================================================================*/

/* Initialize the mempool. */
void mpool_init(mempool *mp, size_t elem_size, size_t nelems_per_block) {
    mp->blocks = NULL;
    mp->nblocks = 0;
    mp->elem_size = elem_size;
    mp->block_size = nelems_per_block;
    mp->_max_offset = elem_size * nelems_per_block;
    mp->_cur_block = (size_t)(-1);
    mp->_cur_offset = mp->_max_offset;
}

/* Destory the mempool. */
void mpool_destory(mempool *mp) {
    size_t i;
    for (i = 0; i < mp->nblocks; i++) {
        free(mp->blocks[i]);
    }
    free(mp->blocks);
}

/* Add n memery blocks to the mempool, each block can hold nelems_per_block
 * elements. */
void mpool_add_n_blocks(mempool *mp, size_t n) {
    if (n == 0) return;
    size_t i, m = mp->nblocks + n;
    mp->blocks = (void **)realloc(mp->blocks, sizeof(void *) * (m));
    assert(mp->blocks);
    for (i = mp->nblocks; i < m; i++) { /* allocate n new blocks */
        mp->blocks[i] = (void *)malloc(mp->_max_offset);
        assert(mp->blocks[i]);
    }
    mp->nblocks += n;
}

/* Allocate memory for an element. */
void *mpool_alloc(mempool *mp) {
    if (mp->_cur_offset == mp->_max_offset) {
        /* if cur_offset reaches the max_offset, that means the current block is
         * used up, start using the next block */
        mp->_cur_offset = 0;
        mp->_cur_block++;
        if (mp->_cur_block == mp->nblocks) {
            /* all blocks are used up, add new blocks */
            mpool_add_n_blocks(mp, 1);
        }
    }
    void *addr = ((char *)mp->blocks[mp->_cur_block]) + mp->_cur_offset;
    mp->_cur_offset += mp->elem_size; /* calc the next element pos */
    return addr;
}

/* Get address of an allocated element. */
void *mpool_at(mempool *mp, size_t index) {
    size_t block = index / mp->block_size;
    size_t offset = index % mp->block_size * mp->elem_size;
    return ((char *)mp->blocks[block]) + offset;
}

/* Get number of elements allocated in the mempool. */
size_t mpool_size(mempool *mp) {
    return mp->block_size * mp->_cur_block + mp->_cur_offset / mp->block_size;
}

/* Get number of elements the mempool can currently hold. */
size_t mpool_capacity(mempool *mp) { return mp->nblocks * mp->block_size; }

/* Check if no elements has been allocated in the mempool. */
bool mpool_empty(mempool *mp) { return mpool_size(mp) == 0; }

/*===========================================================================*/

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
        }
    }

    for (i = 0; i < N; i++) {
        if (arr[i]) {
            assert(*arr[i] == i);
        }
    }
    free(arr);
    mpool_destory(&mpool);
}

int main() {
    srand((unsigned int)time(NULL));

    const int N = 1000;
    int i;
    for (i = 0; i < N; i++) {
        random_test();
    }

    return 0;
}
