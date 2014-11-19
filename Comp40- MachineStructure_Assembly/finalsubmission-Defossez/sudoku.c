/* 
 * sudoku.c
 * By Ashley Hedberg and Dan Defossez, 2/1/2013 
 * HW 2
 *
 * Checks if a sudoku board, in the form of a pgm file is solved or not
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "solution.h"
#include "assert.h"

int main(int argc, char *argv[])
{
        FILE *fp;
        int exit_status;
        assert(argc < 3); /* program can only take 1 or 2 inputs */
        if (argc == 1) {
                exit_status = isSolved(stdin);
        } else {
                fp = fopen(argv[1], "rb");
                assert(fp);
                exit_status = isSolved(fp);
                fclose(fp);
        }
        exit(exit_status);
}
