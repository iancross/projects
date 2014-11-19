/*
*   uarray2.c
*   By Ashley Hedberg and Dan Defossez, 1/29/2013
*   HW 2
*
*   A 2d array implementation based on UArray.
*/

#include "uarray2.h"
#include "uarray.h"
#include <stdlib.h>
#include "assert.h"
#include "mem.h"
#include <stdio.h>
#define T UArray2_T

struct T {
        int width, height, size;
        UArray_T array;
};

int convert_to_1d(int x, int y, T input)
{
        assert(input);
        return ((input->width) * y) + x;
}

int y_from_i(int i, T input)
{
        assert(input);
        return i / (input->width);
}

int x_from_i(int i, T input)
{
        assert(input);
        return i % (input->width);
}

T UArray2_new(int w, int h, int s)
{
        T newArr;
        NEW(newArr);
        newArr->width = w;
        newArr->height = h;
        newArr->size = s;
        newArr->array = UArray_new(w*h, s);
        return newArr;
}

void UArray2_free(T *toDelete)
{
        assert(toDelete);
        assert(*toDelete);
        UArray_free(&((*toDelete)->array));
        FREE(*toDelete);
}

int UArray2_area(T input)
{
        assert(input);
        return (input->width) * (input->height);
}

int UArray2_height(T input)
{
        assert(input);
        return (input->height);
}

int UArray2_width(T input)
{
        assert(input);
        return (input->width);
}

int UArray2_size(T input)
{
        assert(input);
        return input->size;
}

void *UArray2_at(T input, int x, int y)
{
        int pos = convert_to_1d(x, y, input);
        return UArray_at(input->array, pos);
}

void UArray2_map_row_major(T input, void apply(int x, int y,
                                               UArray2_T array2,
                                               void *val, void *cl), 
                           void *cl)
{	
        assert(input);
        int numElems = UArray2_area(input);
        int i;
        for(i = 0; i < numElems; i++) {
                apply(x_from_i(i, input), y_from_i(i, input), input, 
                      UArray_at(input->array, i), cl);
        }
}

void UArray2_map_col_major(T input, void apply(int x, int y, 
                                               UArray2_T array2, 
                                               void *val,
                                               void *cl),          
                           void *cl)
{
        assert(input);
        int x, y;
        for(x = 0; x < input->width; x++) {
                for(y = 0; y < input->height; y++) {
                        apply(x, y, input, UArray2_at(input, x, y), cl); 
                }
        }
}

#undef T
