/*
 *    unblackedges.c
 *    By Ashley Hedberg and Dan Defossez, 2/2/2013
 *    HW 2
 *
 *    Removes black edges from portable bitmap images.
 */

#include "bit2.h"
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "pnmrdr.h"
#include "except.h"
#include "mem.h"

Pnmrdr_T loadMap(FILE *fp, char *program)
{
        Pnmrdr_T rdr;
        TRY
                rdr = Pnmrdr_new(fp);
        EXCEPT(Pnmrdr_Badformat)
                fprintf(stderr, "%s: %s\n", program, "Badly formatted file.");
                fclose(fp);
                Pnmrdr_free(&rdr);
                exit(1);
        END_TRY;
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);
        if (data.type != Pnmrdr_bit) {
                fprintf(stderr, "%s: %s\n", program, "File must be a bitmap.");
                fclose(fp);
                Pnmrdr_free(&rdr);
                exit(1);
        }
        return rdr;
}

Bit2_T readPix(Pnmrdr_T rdr, unsigned width, unsigned height)
{
        Bit2_T image = Bit2_new(width, height);
        unsigned x, y;
        for (y = 0; y < height; y++) {
                for (x = 0; x < width; x++) {
                        Bit2_put(image, x, y, Pnmrdr_get(rdr));
                }
        }
        return image;
}

void neighbors(Stack_T xCoord, Stack_T yCoord, Bit2_T image, int *x, int *y,
               unsigned w, unsigned h, Stack_T toFree)
{
        int *left, *right, *up, *down, width = (int)w, height = (int)h;
        NEW(left);
        NEW(right);
        NEW(up);
        NEW(down);
        *up = *y - 1; /* assign positions of neighboring pixels */
        *down = *y + 1;
        *right = *x + 1;
        *left = *x - 1;
        /* check if out of bounds before getting pixels */
        if (*up >= 0 && Bit2_get(image, *x, *up) == 1) {
                Stack_push(xCoord, x);
                Stack_push(yCoord, up);
                Stack_push(toFree, up);
        } else {
                FREE(up); /* free pointer if not pushed to toFree */
        }
        if (*down < height && Bit2_get(image, *x, *down) == 1) {
                Stack_push(xCoord, x);
                Stack_push(yCoord, down);
                Stack_push(toFree, down);
        } else {
                FREE(down);
        }
        if (*left >= 0 && Bit2_get(image, *left, *y) == 1) {
                Stack_push(xCoord, left);
                Stack_push(yCoord, y);
                Stack_push(toFree, left);
        } else {
                FREE(left);
        }
        if (*right < width && Bit2_get(image, *right, *y) == 1) {
                Stack_push(xCoord, right);
                Stack_push(yCoord, y);
                Stack_push(toFree, right);
        } else {
                FREE(right);
        }
}

void changeImage(Bit2_T image, unsigned width, unsigned height)
{
        Stack_T xCoord = Stack_new();
        Stack_T yCoord = Stack_new();
        Stack_T toFree = Stack_new();
        int *x, *y;
        unsigned i;
        int *xpoint[width], *ypoint[height];
        /* allocate pointers for integers at each coordinate in image */
        for (i = 0; i < height; i++) {
                NEW(ypoint[i]);
                *ypoint[i] = i;
        }
        for (i = 0; i < width; i++) {
                NEW(xpoint[i]);
                *xpoint[i] = i;
        }
        for (i = 0; i < height; i++) { /* get black pix on left/right cols */
                if (Bit2_get(image, 0, i) == 1) {
                        Stack_push(xCoord, xpoint[0]);
                        Stack_push(yCoord, ypoint[i]);
                }
                if (Bit2_get(image, width - 1, i) == 1) {
                        Stack_push(xCoord, xpoint[width - 1]);
                        Stack_push(yCoord, ypoint[i]);
                }
        }
        for (i = 1; i < width - 1; i++) { /* get pix on top/bottom rows */
                if (Bit2_get(image, i, 0) == 1) {
                        Stack_push(xCoord, xpoint[i]);
                        Stack_push(yCoord, ypoint[0]);
                }
                if (Bit2_get(image, i, height - 1) == 1) {
                        Stack_push(xCoord, xpoint[i]);
                        Stack_push(yCoord, ypoint[height - 1]);
                }
        }
        while (Stack_empty(xCoord) != 1) {
                x = Stack_pop(xCoord);
                y = Stack_pop(yCoord);
                /* depth-first search to find all black neighbors */
                if (Bit2_get(image, *x, *y) == 1) {
                        neighbors(xCoord, yCoord, image, x, y, width, height,
                                  toFree);
                        Bit2_put(image, *x, *y, 0);
                }
        }
        /* memory management */
        for (i = 0; i < width; i++) {
                FREE(xpoint[i]);
        }
        for (i = 0; i < height; i++) {
                FREE(ypoint[i]);
        }
        while (Stack_empty(toFree) != 1) {
                x = Stack_pop(toFree);
                FREE(x);
        }
        Stack_free(&toFree);
        Stack_free(&xCoord);
        Stack_free(&yCoord);
}

void printImage(Bit2_T image, Pnmrdr_mapdata data) 
{
        unsigned i, j;
        printf("%s\n%i %i\n", "P1\n# Black edges removed", 
               data.width, data.height);
        for (i = 0; i < data.height; i++) {
                for (j = 0; j < data.width; j++) {
                        printf("%i", Bit2_get(image, j, i));
                        if (j != data.width - 1) {
                                printf(" ");
                        }
                }
                printf("\n");
        }
}

void unblack(FILE *fp, char *program)
{
        Pnmrdr_T rdr = loadMap(fp, program);
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);
        unsigned width = data.width, height = data.height;
        Bit2_T image = readPix(rdr, width, height);
        changeImage(image, width, height);
        printImage(image, data); 
        Pnmrdr_free(&rdr);
        Bit2_free(&image);
}

int main(int argc, char *argv[])
{
        FILE *fp;
        if (argc == 1) {
                unblack(stdin, argv[0]);
        } else if (argc == 2) {
                fp = fopen(argv[1], "rb");
                if (fp == NULL) {
                        fprintf(stderr, "%s: %s\n", argv[0], "Invalid file.");
                }
                unblack(fp, argv[0]);
                fclose(fp);
        } else { /* exit - too many arguments */
                fprintf(stderr, "%s: %s\n", argv[0], "Too many arguments.");
                exit(1);
        }
        return 0;
}
