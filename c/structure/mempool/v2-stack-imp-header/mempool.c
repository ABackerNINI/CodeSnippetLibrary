/** File: mempool.c
 *  Tags: c,structure,mempool
 *
 *  Desc: Use it when you need to allocate a lot of fixed size memory blocks.
 *      Compared with the v1-simple version, these memory blocks can be freed at
 *      any time, the drawback is each memory block will contain an eight bytes
 *      (4 for 32 bits system) header.
 *
 *      Both mempool_alloc() and mempool_free() have O(1) time complexity.
 *
 *  Date: 2020/11/8
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

#define MEMPOOL_MEM_ALIGNMENT 16 /* should be power of 2 */
#define HEADER_SIZE sizeof(mempool_pack)
#define INIT_OFFSET (MEMPOOL_MEM_ALIGNMENT - HEADER_SIZE)

typedef struct {
    size_t block_no;   /* the header */
    /* char ptr[0]; */ /* memory alignment? */
} mempool_pack;

typedef struct {
    void *block;
    size_t ref;
} mempool_block;

typedef struct {
    mempool_block **blocks; /* data blocks */
    size_t nblocks;         /* how many data blocks */
    size_t elem_size;       /* element size in bytes */
    size_t block_size;      /* how many elements a block can hold */
    size_t size;            /* elements allocated */
    stack_t free_blocks;

    /* The next element position is (char*)blocks[_cur_block] + _cur_offset */

    size_t _max_offset; /* block size in bytes */
    size_t _cur_block;  /* current block */
    size_t _cur_offset; /* block offset */
} mempool_t;

void mpool_init(mempool_t *mp, size_t elem_size, size_t nelems_per_block);
void mpool_destory(mempool_t *mp);
void mpool_add_n_blocks(mempool_t *mp, size_t n);
void *mpool_alloc(mempool_t *mp);
void mpool_free(mempool_t *mp, void *ptr);
size_t mpool_size(mempool_t *mp);
size_t mpool_capacity(mempool_t *mp);
bool mpool_empty(mempool_t *mp);

static size_t round_up(size_t size, size_t align);
static mempool_block *new_block(size_t size);
static void free_block(mempool_block *block);

/*===========================================================================*/

/* Initialize the mempool. */
void mpool_init(mempool_t *mp, size_t elem_size, size_t nelems_per_block) {
    /* for memory alignment */
    elem_size = round_up(elem_size + HEADER_SIZE, MEMPOOL_MEM_ALIGNMENT);

    mp->blocks = NULL;
    mp->nblocks = 0;
    mp->elem_size = elem_size;
    mp->block_size = nelems_per_block;
    mp->size = 0;
    stk_init(&mp->free_blocks);
    mp->_max_offset = elem_size * nelems_per_block + INIT_OFFSET;
    mp->_cur_block = 0;
    mp->_cur_offset = mp->_max_offset;
}

/* Destory the mempool. */
void mpool_destory(mempool_t *mp) {
    size_t i;
    for (i = 0; i < mp->nblocks; i++) {
        free_block(mp->blocks[i]);
    }
    free(mp->blocks);
    stk_destory(&mp->free_blocks);
}

/* Add n memery blocks to the mempool, each block can hold nelems_per_block
 * elements. */
void mpool_add_n_blocks(mempool_t *mp, size_t n) {
    if (n == 0) return;
    size_t i, m = mp->nblocks + n;
    mp->blocks =
        (mempool_block **)realloc(mp->blocks, sizeof(mempool_block *) * (m));
    assert(mp->blocks);
    /* allocate n new blocks */
    for (i = mp->nblocks; i < m; i++) {
        mp->blocks[i] = new_block(mp->_max_offset);
    }
    /* push these new blocks to free_blocks backward, so the first new block is
     * on top and will be used first */
    for (i = m - 1;; i--) {
        stk_push(&mp->free_blocks, i);
        if (i == mp->nblocks) break;
    }
    mp->nblocks += n;
}

/* Allocate memory for an element. */
void *mpool_alloc(mempool_t *mp) {
    if (mp->_cur_offset == mp->_max_offset) {
        /* if cur_offset reaches the max_offset, that means the current block is
         * used up, start using the next block */
        mp->_cur_offset = INIT_OFFSET;
        if (stk_empty(&mp->free_blocks)) {
            /* all blocks are used up, add new blocks */
            mpool_add_n_blocks(mp, 1);
        }
        mp->_cur_block = stk_top(&mp->free_blocks);
        stk_pop(&mp->free_blocks);
    }
    mempool_block *block = mp->blocks[mp->_cur_block];
    mempool_pack *pack =
        (mempool_pack *)(((char *)block->block) + mp->_cur_offset);
    mp->_cur_offset += mp->elem_size; /* calc the next element pos */
    pack->block_no = mp->_cur_block;  /* set header */
    ++(block->ref);
    ++(mp->size);
    return (char *)pack + HEADER_SIZE; /* return the ptr */
}

/* Free memory of an element pointer returned by mpool_alloc(). */
void mpool_free(mempool_t *mp, void *ptr) {
    if (ptr == NULL) return;
    mempool_pack *pack = ptr - HEADER_SIZE;
    assert(pack->block_no < mp->nblocks);
    mempool_block *block = mp->blocks[pack->block_no];
    assert(block->ref);
    if (--(block->ref) == 0) {
        if (mp->_cur_block == pack->block_no) {
            /* if this is the currently using block, reset the offset */
            mp->_cur_offset = INIT_OFFSET;
        } else {
            /* push to the free_blocks stack */
            stk_push(&mp->free_blocks, pack->block_no);
        }
    }
    pack->block_no = (size_t)-1; /* to avoid double free */
}

/* Get number of elements allocated in the mempool. */
size_t mpool_size(mempool_t *mp) { return mp->size; }

/* Get number of elements the mempool can currently hold. */
size_t mpool_capacity(mempool_t *mp) { return mp->nblocks * mp->block_size; }

/* Check if no elements has been allocated in the mempool. */
bool mpool_empty(mempool_t *mp) { return mpool_size(mp) == 0; }

/*===========================================================================*/

/* Round up value to the next multiple of 'align' which should be a power of 2.
 */
static size_t round_up(size_t val, size_t align) {
    return (val + align - 1) & (~(align - 1));
}

/* Allocate a mempool_block. */
static mempool_block *new_block(size_t size) {
    mempool_block *block = (mempool_block *)malloc(sizeof(mempool_block));
    assert(block);
    block->block = (void *)malloc(size);
    assert(block->block);
    block->ref = 0;
    return block;
}

/* Free a mempool block. */
static void free_block(mempool_block *block) {
    free(block->block);
    free(block);
}

/*===========================================================================*/

void random_test() {
    mempool_t mpool;
    mpool_init(&mpool, sizeof(int), 100);
    int i, N = rand() % 10000 + 1;
    int **arr = malloc(sizeof(int *) * N);
    memset(arr, 0, sizeof(int *) * N);
    for (i = 0; i < N; i++) {
        if (rand() % 2 == 0) {
            int *p = mpool_alloc(&mpool);
            *p = i;
            assert(((size_t)p & (MEMPOOL_MEM_ALIGNMENT - 1)) == 0);
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

    const int N = 1000;
    int i;
    for (i = 0; i < N; i++) {
        random_test();
    }

    return 0;
}
