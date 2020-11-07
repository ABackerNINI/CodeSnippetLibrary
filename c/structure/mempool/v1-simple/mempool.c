/** File: mempool.c
 *  Tags: c,structure,mempool
 *
 *  Desc: Use it when you need to allocate a lot of fixed size blocks, and thoes
 *      blocks are not going to be freed until the end.
 *
 *  2020/11/8
 *
 *  Compile with: gcc mempool.c -W -Wall -Werror -o mempool.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

/*===========================================================================*/

#define DEFAULT_MEMPOOL_CAPACITY 64

typedef struct {
    void **blocks;     /* data blocks */
    size_t nblocks;    /* how many data blocks */
    size_t elem_size;  /* element size in bytes */
    size_t block_size; /* how many elements a block can hold */

    /* Can use 'size' to represent all the following three members, but to avoid
     * repeated calculation, use them */
    /* The next element position is (char*)blocks[_cur_block] + block_offset */

    size_t _cur_block;  /* current block */
    size_t _cur_offset; /* block offset */
    size_t _max_offset; /* block size in bytes */
} mempool;

void mpool_init(mempool *mp, size_t elem_size, size_t nelems_per_block);
void mpool_destory(mempool *mp);
void mpool_add_n_blocks(mempool *mp, size_t n);
void *mpool_alloc(mempool *mp);
size_t mpool_size(mempool *mp);
size_t mpool_capacity(mempool *mp);
bool mpool_empty(mempool *mp);

/*===========================================================================*/

void mpool_init(mempool *mp, size_t elem_size, size_t nelems_per_block) {
    mp->blocks = NULL;
    mp->nblocks = 0;
    mp->elem_size = elem_size;
    mp->block_size = nelems_per_block;
    mp->_cur_block = 0;
    mp->_cur_offset = 0;
    mp->_max_offset = elem_size * nelems_per_block;
}

void mpool_destory(mempool *mp) {
    size_t i;
    for (i = 0; i < mp->nblocks; i++) {
        free(mp->blocks[i]);
    }
    free(mp->blocks);
}

void mpool_add_n_blocks(mempool *mp, size_t n) {
    size_t i;
    mp->blocks = (void **)malloc(sizeof(void *) * (mp->nblocks + n));
    assert(mp->blocks);
    for (i = 0; i < n; i++) { /* allocate n new blocks */
        mp->blocks[i + mp->nblocks] = (void *)malloc(mp->_max_offset);
        assert(mp->blocks[i + mp->nblocks]);
    }
    mp->nblocks += n;
}

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

size_t mpool_size(mempool *mp) {
    return mp->block_size * mp->_cur_block + mp->_cur_offset / mp->block_size;
}

size_t mpool_capacity(mempool *mp) { return mp->nblocks * mp->block_size; }

bool mpool_empty(mempool *mp) { return mpool_size(mp) == 0; }

/*===========================================================================*/

int main(int argc, char **argv) {
    printf("Hello world.\n");

    return 0;
}
