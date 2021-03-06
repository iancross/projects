/*      brightness.c
 *      by Ian Cross and Alison Tai, 1/20/2013
 *      Homework 1: Understanding and Using INterfaces
 *      
 *      Computes the average brightness of a portable graymap file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>

/*Function Declarations*/
void CheckFile(FILE *fp);
void AvgBright(Pnmrdr_T rdr);
int SumBright(Pnmrdr_T rdr, int height, int width);

/*
 * Reads in file from stdin or file based on the number of arguments
 * given. If there is an incorrect commandline format, an error 
 * message is printed out.
 */
int main(int argc, char** argv)
{
        FILE *fp;

        if (argc == 1) {
                fp = stdin;
                CheckFile(fp);
        } else if (argc == 2) {
                fp = fopen(argv[1],"rb");
                if (fp == NULL) {
                        fprintf(stderr, "%s: %s %s %s\n", 
                                argv[0], "Could not open file ",
                                argv[1], "for reading");
                        exit(1);                
                } else {
                        CheckFile(fp);
                }
        } else {
                fprintf(stderr, "Incorrect Commandline format\n");
        }
}

/* Checks if the given file is a portable graymap file.*/
void CheckFile(FILE *fp)
{
        Pnmrdr_mapdata data;
        Pnmrdr_T rdr = Pnmrdr_new(fp);

        data = Pnmrdr_data(rdr);

        /*Checks if file is a portable graymap file (type index = 2)*/
        if (data.type == 2) {
                AvgBright(rdr);
        } else {
                fprintf(stderr, "Error: not a graymap image");
        }

        fclose(fp);
}

/*
 * Calculates and prints out the average brightness of a portable 
 * graymap file (PGM) given the reader data of a PGM.
 */
void AvgBright(Pnmrdr_T rdr)
{
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);
 
        int width = data.width;
        int height = data.height;
        int maxval = data.denominator;
        int TotPix = width * height;

        int sum = SumBright(rdr, height, width);
        float avg = ( (float)sum / (float)maxval ) / (float)TotPix;
        printf("%0.3f \n", avg); 

        Pnmrdr_free(&rdr);
}

/*
 * Returns the sum of the brightness of a PGM given the reader
 * data, height in pixels, and width in pixels of a PGM.
 */
int SumBright(Pnmrdr_T rdr, int height, int width)
{
        int pix_sum = 0;

        for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                        pix_sum = pix_sum + Pnmrdr_get(rdr);
                }
        }

        return pix_sum;
}

