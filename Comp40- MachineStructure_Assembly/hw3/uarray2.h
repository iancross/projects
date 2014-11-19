/*
*    uarray2.h
*    By Ashley Hedberg and Dan Defossez, 1/29/2013
*    HW2
*   
*    Interface for UArray2
*/

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#define T UArray2_T
typedef struct T *T;

extern T    UArray2_new  (int width, int height, int size);
extern void UArray2_free (T *uarray2);
extern int  UArray2_area (T uarray2);
extern int  UArray2_width(T uarray2);
extern int  UArray2_height(T uarray2);
extern int  UArray2_size (T uarray2);
extern void *UArray2_at  (T uarray2, int x, int y);
extern void UArray2_map_row_major(T uarray2, 
                                  void apply(int x, int y, T uarray2, 
				             void *val, void *cl), 
                                  void *cl);
extern void UArray2_map_col_major(T uarray2, 
                                  void apply(int x, int y, T uarray2, 
				 	     void *val, void *cl), 
                                  void *cl);

#undef T
#endif
