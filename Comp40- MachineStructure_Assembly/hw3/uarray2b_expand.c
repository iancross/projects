/*
 * uarray2b.c
 * Ian Cross and Dan Defossez
 * 2/8/2013
 * Implementation of 2D blocked array
 */

#include "uarray2b.h"
#include "uarray2.h"
#include "mem.h"
#include "assert.h"
#include <math.h>
#include <stdio.h>

/*Note: i is column and j is row*/
/*Note: our implementation of UArray2b is a UArray2 of UArray2s.*/

#define T UArray2b_T
#define CACHESIZE 65536

int block_dim(int length, int blocksize);
void make_arrs(int col, int row, UArray2_T, void *val, void *cl);
T initialize_arr(int width, int height, int size, int blocksize);
void block_iter(UArray2b_T array2b, int i, int j, 
                void apply(int i, int j, T array2b, void *elem, void *cl),
                void *cl);
void free_blocks(int col, int row, UArray2_T array2, void *val, void *cl);

struct T {
        int height, width, size, blocksize; 
        UArray2_T shell;
};

struct element_size {
        int size;
        int blocksize;
};

/* creates a new blocked 2D array with the given dimensions and blocksize
 * returns a UArray2b.
 */
T UArray2b_new (int width, int height, int size, int blocksize)
{
        return initialize_arr(width, height, size, blocksize);
}

/* creates a new blocked 2D array with the given dimensions and blocksize
 * is such that each block is as close to 64KB as possible
 */
T UArray2b_new_64K_block(int width, int height, int size)
{
        int blocksize;
        blocksize = (int)sqrt(CACHESIZE/size);
        return initialize_arr(width, height, size, blocksize);
}

/* Takes in a pointer to an array2b and frees all of its allocated memory */
void UArray2b_free(T *array2b)
{
        assert(array2b);
        assert(*array2b);
        UArray2_map_row_major((*array2b)->shell, free_blocks, NULL);
        UArray2_free(&((*array2b)->shell));
        FREE(*array2b);
}

/* returns the width of the array2b */
int UArray2b_width (T array2b)
{
        assert(array2b);
        return array2b->width;
}

/* returns the height of the array2b */
int UArray2b_height(T array2b)
{
        assert(array2b);
        return array2b->height;
}

/* returns the element size of the array2b */
int UArray2b_size (T array2b)
{
        assert(array2b);
        return array2b->size;
}

/* returns the blocksize of the array2b */
int UArray2b_blocksize(T array2b)
{
        assert(array2b);
        return array2b->blocksize;
}

/* returns a void pointer to whatever is stored in location (i,j) in the
 * abstract 2D array
 */
void *UArray2b_at(T array2b, int i, int j)
{
        assert(array2b);
        int x1, y1, x2, y2;
        UArray2_T *inner;
        x1 = i / array2b->blocksize;
        y1 = j / array2b->blocksize;
        x2 = i % array2b->blocksize;
        y2 = j % array2b->blocksize;
        inner = UArray2_at(array2b->shell, x1, y1);/* pointer to the block */
        return UArray2_at(*inner, x2, y2);
}

/* applies a client provided apply function to each element in the array2b 
 * in a block-major fashion.
 */
void UArray2b_map(T array2b, void apply(int i, int j, T array2b,
                                        void *elem, void *cl), void *cl)
{
        assert(array2b);
        int row, col;
        int w = array2b->width;
        int h = array2b->height;
        int bs = array2b->blocksize;
        for (row = 0; row < block_dim(h, bs); row++){
                for(col = 0; col < block_dim(w, bs); col++){
                        /* now iterate through each block */
                        block_iter(array2b, col, row, apply, cl);
                }
        }
}

/* Used by UArray2b_map. This internal function returns how many blocks 
 * there are from a given dimension (either width or height)
 */     
int block_dim(int length, int blocksize)
{
        if (blocksize >= length) {
                return 1;
        }
        if (length % blocksize == 0){
                return length/blocksize;
        }else {
                return length/blocksize + 1;
        }
}

/* initialize each block within the shell UArray2 */
void make_arrs(int col, int row, UArray2_T a, void *val, void *cl)
{
        (void) col;
        (void) row;
        (void) a;
        struct element_size *e_s = cl;
        UArray2_T *inner;
        inner = val;
        *inner = UArray2_new(e_s->blocksize, e_s->blocksize, e_s->size);
}

/* Internally used by the two UArray2b_new functions
 * Initializes each element in the UArray2b
 */
T initialize_arr(int width, int height, int size, int blocksize)
{
        T newArr;
        NEW(newArr); /* "shell" array */
        newArr->height = height;
        newArr->width = width;
        newArr->size = size;
        newArr->blocksize = blocksize;

        width = block_dim(width, blocksize);
        height = block_dim(height, blocksize);
        newArr->shell = UArray2_new(width, height, sizeof(UArray2_T));
        /* uarray2 of uarray2s, for blocks */

        struct element_size es;
        es.size = size;
        es.blocksize = blocksize;
        UArray2_map_row_major(newArr->shell, make_arrs, &es);
        return newArr;
}

/* iterates through the elements in each block and calls apply. Used by
 * UArray2b_map
 */
void block_iter(UArray2b_T array2b, int i, int j, 
                void apply(int i, int j, T array2b, void *elem, void *cl),
                void *cl)
{
        void* elem;
        int client_x, client_y, col, row, bs = array2b->blocksize;
        for (row = 0; row < bs; row++) {
                for (col = 0; col < bs; col++) {
                        client_x = i*bs + col; /* coordinates of client */
                        client_y = j*bs + row;
                        elem = UArray2b_at(array2b, client_x, client_y);
                        if(client_x < array2b->width &&
                           client_y < array2b->height){
                                apply(client_x, client_y, array2b, elem, cl);
                        }
                }
        }
        
}

/* Frees each block in the shell array */
void free_blocks(int col, int row, UArray2_T a2, void *elem, void *cl)
{
        (void)col;
        (void)a2;
        (void)row;
        (void)elem;
        (void)cl;
        UArray2_T *array2 = elem;
        UArray2_free(array2);
}
