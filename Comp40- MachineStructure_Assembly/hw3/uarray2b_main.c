#include "uarray2b.h"
#include "uarray2.h"
#include <stdio.h>

void print(int i, int j, UArray2b_T array2b, void *elem, void *cl);

int main()
{
    int *index;
    int k = 0;
    UArray2b_T array = UArray2b_new_64K_block(24, 24, 400);
    UArray2b_at(array, 50, 50);
      for(int i = 0; i < 24; i++){
   	for(int j = 0; j < 24; j++){
	    index = UArray2b_at(array, j, i);
	    *index = k;
	    k++;
	}
      }
    UArray2b_map(array, print, NULL);
    UArray2b_free(&array);
}
void print(int i, int j, UArray2b_T array2b, void *elem, void *cl) 
{
    (void)array2b;
    (void)cl;
    int *e = elem;
    printf("%s %i,%i %s %i\n", "At location", i, j, "Value is: ", *e);
}
