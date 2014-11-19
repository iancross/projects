/*
 * uarray2b.h
 * Ian Cross and Dan Defossez
 * 2/8/2013
 * Interface of 2D blocked array
 */

#ifndef UARRAY2B_INCLUDED
#define UARRAY2B_INCLUDED

#define T UArray2b_T
typedef struct T *T;


extern T UArray2b_new (int width, int height, int size, int blocksize);
extern T UArray2b_new_64K_block(int width, int height, int size);
extern void UArray2b_free(T *array2b);
extern int UArray2b_width (T array2b);
extern int UArray2b_height(T array2b);
extern int UArray2b_size (T array2b);
extern int UArray2b_blocksize(T array2b);
extern void *UArray2b_at(T array2b, int i, int j);
extern void UArray2b_map(T array2b, void apply(int i, int j, T array2b,
					      void *elem, void *cl), void *cl);

#undef T
#endif
