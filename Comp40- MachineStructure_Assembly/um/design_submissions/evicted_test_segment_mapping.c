#include <stdio.h>
#include "segMem.h"
#include "assert.h"        

#define SIZE 12
#define ARBITRARY_NUMBER 13
#define HINT 1
#define ID 1
#define INDEX 0
#define STORE 15
#define LOAD 233 //this is arbitrary, it is overwritten to 15

void print_arr(uint32_t* array);

int main(){
        Seq_T myTestSeq = Seq_new(HINT);
        Seq_T myFreedMem = Seq_new(HINT);
        int regs[8] = {0, 0, LOAD, STORE, INDEX, ID, ARBITRARY_NUMBER, SIZE};
        segMem_map(myTestSeq, myFreedMem, regs, 7, 6);
        segment_data seg_struct = (segment_data) Seq_get(myTestSeq, 0);


        segMem_map(myTestSeq, myFreedMem, regs, 7, 6);
        printf("penis cirCUMference: %i feet\n", Seq_length(myTestSeq));

        seg_struct = (segment_data) Seq_get(myTestSeq, 1);
        segMem_store(myTestSeq, regs, 5, 4, 3);
        
        print_arr(seg_struct->segment);
        //segMem_load(myTestSeq, regs, 2, 5, 4);
        //printf("loaded item is %i and should be 15", regs[2]);

        //segMem_unmap(myTestSeq, myFreedMem, regs, 5);
}

void print_arr(uint32_t* array){
        for(int i = 0; i < SIZE; i++){
                printf("%d ", array[i]);
        }
}
