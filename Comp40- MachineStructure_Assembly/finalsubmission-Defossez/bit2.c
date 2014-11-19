/*
*   bit2.c
*   By Ashley Hedberg and Dan Defossez, 1/29/2013
*   Homework 2
*   
*   A two-dimensional bit vector interface based on Hanson's Bit interface
*/

#include "bit2.h"
#include "bit.h"
#include <stdlib.h>
#include "assert.h"
#include "mem.h"
#define T Bit2_T

struct T {
        int width, height;
        Bit_T vec;
};


int convert_to_1d(T input, int x, int y)
{
        assert(input);
        return ((input->width) * y) + x;
}

int y_from_i(T input, int i)
{
        assert(input);
        return i / (input->width);
}

int x_from_i(T input, int i)
{
        assert(input);
        return i % (input->width);
}

T Bit2_new(int w, int h)
{
        T newVec;
        NEW(newVec);
        newVec->width = w;
        newVec->height = h;
        newVec->vec = Bit_new(w * h);
        return newVec;
}

void Bit2_free(T *toDelete)
{
        assert(toDelete);
        assert(*toDelete);
        Bit_free(&((*toDelete)->vec));
        FREE(*toDelete);
}

int Bit2_area(T input)
{
        assert(input);
        return (input->width) * (input->height);
}

int Bit2_put(T input, int x, int y, int bit)
{
        assert(input);
        int pos = convert_to_1d(input, x, y);
        return Bit_put(input->vec, pos, bit);
}

int Bit2_get(T input, int x, int y)
{
        assert(input);
        int pos = convert_to_1d(input, x, y);
        return Bit_get(input->vec, pos);
}


void Bit2_map_row_major(T input, void apply(int x, int y, int bit,
                           void *cl), void *cl)
{
        assert(input);
        int numElems = Bit2_area(input);
        int i, bit;
        for(i = 0; i < numElems; i++) {
                bit = Bit_get(input->vec, i);
                apply(x_from_i(input, i), y_from_i(input, i), bit, cl);
        }
}

void Bit2_map_col_major(T input, void apply(int x, int y, int bit,
                           void *cl), void *cl)
{
        assert(input);
        int x, y, index, bit;
        for(x = 0; x < input->width; x++) {
                for(y = 0; y < input->height; y++) {
                        index = convert_to_1d(input, x, y);
                        bit = Bit_get(input->vec, index);
                        apply(x, y, bit, cl); 
                }
        }

}

#undef T
