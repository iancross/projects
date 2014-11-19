/*
 * Ian Cross and Dan Defossez
 * ppmtrans.c - Image Rotation
 * 2/15/13
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include <a2methods.h>
#include "a2plain.h"
#include "a2blocked.h"
#include <pnm.h>
#define TRAN 45  /* constant to identify transpose option */



typedef A2Methods_UArray2 A2;
typedef A2Methods_mapfun mf;

Pnm_ppm load_image(FILE *fp, A2Methods_T methods);
A2 rotate_image(Pnm_ppm img, A2Methods_T methods, int rotation, mf *map);
void copy_pixels(int i, int j, A2 array2, void *elem, void *cl);
void rotate_apply(int i, int j, A2 array2, void *elem, void *cl);
void print_apply(int i, int j, A2 array2, void *elem, void *cl);
void print_final(Pnm_ppm img, A2 final_arr, A2Methods_T methods);
void free_mem(FILE *fp, Pnm_ppm img);
struct rotate_cl {
        A2 dest;
        A2Methods_T methods;
        int deg, old_w, old_h;
};

int main(int argc, char *argv[])
{
        int rotation = 0;
        int rot_bool = 0;    /* bools for flags */
        int input_bool = 0;
        Pnm_ppm img;
        A2 final_img = NULL;
        
        A2Methods_T methods = uarray2_methods_plain; /* default to UArray2 
                                                      * methods
                                                      */
        assert(methods);
        A2Methods_mapfun *map = methods->map_default; /* default to best map */
        assert(map);

#define SET_METHODS(METHODS, MAP, WHAT) do {                            \
                methods = (METHODS);                                    \
                assert(methods);                                        \
                map = methods->MAP;                                     \
                if (!map) {                                             \
        fprintf(stderr, "%s does not support " WHAT "mapping\n", argv[0]);\
                        exit(1);                                        \
                }                                                       \
        } while(0)

        FILE *fp = 0;
        int i;
        if (argc == 1) {
                fprintf(stderr, "%s\n", "Error, no arguments provided.");
                exit(1);
        }
        for (i = 1; i < argc; i++) {
                if (!strcmp(argv[i], "-row-major")) {
                        SET_METHODS(uarray2_methods_plain,
                                    map_row_major,
                                    "row-major");
                } else if (!strcmp(argv[i], "-col-major")) {
                        SET_METHODS(uarray2_methods_plain,
                                    map_col_major,
                                    "column-major");
                } else if (!strcmp(argv[i], "-block-major")) {
                        SET_METHODS(uarray2_methods_blocked,
                                    map_block_major,
                                    "block-major");
                } else if (!strcmp(argv[i], "-rotate")) {
                        assert(i + 1 < argc);
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        assert(*endptr == '\0'); /* parsed all correctly */
                        assert(rotation == 0   || rotation == 90
                               || rotation == 180 || rotation == 270);
                        rot_bool = 1;
                } else if (!strcmp(argv[i], "-transpose")) {
                        rotation = TRAN; /* code for transposition */
                        rot_bool = 1;
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n",
                                argv[0], argv[i]);
                        exit(1);
                
                } else if (i == argc - 1) {
                        input_bool = 1;
                        fp = fopen(argv[i], "r");
                        assert(fp);
                } else if (argc - i > 2) {
                        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                                "[-{row,col,block}-major] [filename]\n",
                                argv[0]);
                        exit(1);
                } else {
                        break;
                }
        }
        
        if (input_bool == 0) {
                fp = stdin;
        }
        img = load_image(fp, methods);
        if (rot_bool == 1) {
                final_img = rotate_image(img, methods, rotation, map);
        }
        else{
                fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n", argv[0]);
                free_mem(fp, img); 
                exit(1);
        }     
        print_final(img, final_img, methods);   
        free_mem(fp, img);
}

/* loads the image using the Pnm reader and returns a Pnm_ppm struct*/
Pnm_ppm load_image(FILE *fp, A2Methods_T methods)
{
        Pnm_ppm to_return;
        to_return = Pnm_ppmread(fp, methods);
        return to_return;
}

/* rotates the image using the specified mapping function*/
A2 rotate_image(Pnm_ppm img, A2Methods_T methods, int rotation, mf *map)
{
        struct rotate_cl cl;
        cl.methods = methods;
        int width = img->width, height = img->height;
        int size = methods->size(img->pixels);
        if ((rotation / 90) % 2 == 1 || rotation == TRAN){ 
        /* only swap w, h if rotating 90
         * or 270 degrees, or transpose */
                cl.dest = methods->new(height, width, size);    
        } else {
                cl.dest = methods->new(width, height, size);
        }
        cl.deg = rotation;
        cl.old_w = img->width;
        cl.old_h = img->height;
        map(img->pixels, rotate_apply, &cl);
        return cl.dest;
}

/*
 * prints the final rotated image using Pnm_ppmwrite. Precondition is that 
 * the image has already been rotated and is ready to be printed
 */
void print_final(Pnm_ppm img, A2 final, A2Methods_T methods)
{       
        A2 to_delete = img->pixels;
        methods->free(&to_delete);
        img->pixels = final;
        assert(img->pixels);
        img->width = methods->width(final);
        img->height = methods->height(final);
        Pnm_ppmwrite(stdout, img);      
}

/* 
 * This is the apply function used by rotate_image which copies the current 
 * pixel struct into the proper location in the new rotated image using a 
 * conversion formula
 */
void rotate_apply(int i, int j, A2 array2, void *elem, void *cl)
{
        (void)array2;
        struct rotate_cl *closure;
        closure = cl;
        int deg = closure->deg;
        A2Methods_T methods = closure->methods;
        int old_h = closure->old_h; /* our calculations use the dimensions  */
        int old_w = closure->old_w; /* of the original image, and we don't  */
        A2 dest = closure->dest;    /* know whether the new image will have */
        Pnm_rgb pix = elem;         /* different dimensions or not, so we   */
        Pnm_rgb dest_pix;           /* passed in the old dimensions         */
        if (deg == 90) {       
                dest_pix = methods->at(dest, old_h-j-1, i); 
                memcpy(dest_pix, pix, sizeof(*pix));

        } else if (deg == 180) {
                dest_pix = methods->at(dest, old_w-i-1, old_h-j-1);
                memcpy(dest_pix, pix, sizeof(*pix));
        } else if (deg == 270) {
                dest_pix = methods->at(dest, j, old_w-i-1); 
                memcpy(dest_pix, pix, sizeof(*pix));
        } else if (deg == 0) { 
                dest_pix = methods->at(dest, i, j); 
                memcpy(dest_pix, pix, sizeof(*pix));
        } else if (deg == TRAN) {                
                dest_pix = methods->at(dest, j, i); 
                memcpy(dest_pix, pix, sizeof(*pix));
        }

        
}

/* Frees allocated memory */
void free_mem(FILE *fp, Pnm_ppm img)
{
        fclose(fp);
        Pnm_ppmfree(&img);
}
