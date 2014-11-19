/*
 *  solution.c
 *  By Dan Defossez and Ashley Hedberg, 2/1/2013
 *  HW 2
 *
 *  Implementation of the sudoku solution checker.
 *
 */

#include "uarray2.h"
#include <stdlib.h>
#include <stdio.h>
#include "pnmrdr.h"
#include "assert.h"
#include "mem.h"
#define DIMENSION 9

struct closure {
        int line[DIMENSION], digits[DIMENSION];
        int exit_status, isRow;
};

Pnmrdr_T loadMap(FILE *fp)
{
        Pnmrdr_T rdr = Pnmrdr_new(fp);
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);
        assert(data.type == Pnmrdr_gray);
        return rdr;
}

UArray2_T loadArray(Pnmrdr_T p, int *i)
{
        int x, y;
        unsigned *val;
        UArray2_T u = UArray2_new(DIMENSION, DIMENSION, sizeof(*val));
        for (x = 0; x < DIMENSION; x++) {
                for(y = 0; y < DIMENSION; y++) {
                        val = UArray2_at(u, x, y);
                        *val = Pnmrdr_get(p);
                        if(*val > DIMENSION || *val == 0) { 
                                *i = 1;
                        }
                }
        }
        return u;
}

void reset(int arr[DIMENSION])
{
        int i;
        for (i = 0; i < DIMENSION; i++) {
                arr[i] = 0;
        }
}

void checkLine(int x, int y, void *val, void *cl)
{
        int i;    
        unsigned *value = val;
        struct closure *close = cl;
        if (close->isRow == 1) {
                i = y;
        } else {
                i = x;
        }
        if (close->line[i] == 0) {
                close->line[i] = 1;
                reset(close->digits); 
        }
        if (close->digits[*value - 1] != 0) {
                close->exit_status = 1;
        }
        close->digits[*value - 1] = 1; 
}

int checkBox(UArray2_T u, struct closure *cl, int lx, int ux, int ly, int uy)
{
        int i, j;
        unsigned *val;
        for (i = lx; i <= ux; i++) {
                for (j = ly; j <= uy; j++) {
                        val = UArray2_at(u, i, j);
                        if (cl->digits[*val - 1] != 0) {
                                return 1; /* have seen this value in box */
                        }
                        cl->digits[*val - 1] = 1;
                }
        }
        return 0; /* didn't encounter repeat values */
}

int checkThrees(UArray2_T u, struct closure *cl)
{
        int lx, ux, ly, uy;
        for (lx = 0; lx <= 6; lx += 3) {
                for (ly = 0; ly <= 6; ly += 3) {
                        ux = lx + 2;
                        uy = ly + 2;
                        cl->exit_status = checkBox(u, cl, lx, ux, ly, uy);
                        if (cl->exit_status == 1) {
                                return 1;
                        }
                        reset(cl->digits);      
                }
        }
        return 0;
}

int checkPuzz(UArray2_T u)
{
        int toReturn;
        struct closure *cl;
        NEW(cl);
        cl->exit_status = 0;
        reset(cl->line);
        reset(cl->digits); /* check rows */
        cl->isRow = 1;
        UArray2_map_row_major(u, checkLine, cl);
        if (cl->exit_status == 1) {
                toReturn = cl->exit_status;
                FREE(cl);
                return toReturn;
        } /* check columns */
        cl->isRow = 0;
        reset(cl->line);
        reset(cl->digits);
        UArray2_map_col_major(u, checkLine, cl);
        if (cl->exit_status == 1) {
                toReturn = cl->exit_status;
                FREE(cl);
                return toReturn;
        } /* check 3x3 */
        reset(cl->line);
        reset(cl->digits);
        cl->exit_status = checkThrees(u, cl);
        toReturn = cl->exit_status;
        FREE(cl);
        return toReturn;
}

int isSolved(FILE *fp)
{
        int invalidPix = 0;
        int exit_status = 1;
        Pnmrdr_T puzzle = loadMap(fp);
        Pnmrdr_mapdata data = Pnmrdr_data(puzzle);
        if (data.width != DIMENSION || data.height != DIMENSION ||
            data.denominator != DIMENSION) {
                Pnmrdr_free(&puzzle);
                return 1;
        }
        UArray2_T puzzArray = loadArray(puzzle, &invalidPix);
        if (invalidPix == 1) {
                exit_status  = 1;         
        } else {
                exit_status = checkPuzz(puzzArray);
        }
        UArray2_free(&puzzArray);
        Pnmrdr_free(&puzzle);
        return exit_status;
}
