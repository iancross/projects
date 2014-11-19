/*
*    bit2.h
*    By Ashley Hedberg and Dan Defossez, 1/29/2013
*    HW2
*   
*    Interface for Bit2
*/

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

#define T Bit2_T
typedef struct T *T;

extern T    Bit2_new (int width, int height);
extern void Bit2_free(T *bit2);
extern int  Bit2_area(T bit2);
extern int  Bit2_get (T bit2, int x, int y);
extern int  Bit2_put (T bit2, int x, int y, int bit);
extern void Bit2_map_row_major(T bit2, 
                               void apply(int x, int y, int bit, void *cl), 
                               void *cl);
extern void Bit2_map_col_major(T bit2, 
                               void apply(int x, int y, int bit, void *cl), 
                               void *cl);

#undef T
#endif
