#include <stdio.h>
#include "segMem.h"
        
#define SIZE 12
#define ID 13
#define HINT 15

int main(){
        Seq_T myTestSeq = Seq_new(HINT);
        segMem_map(myTestSeq, SIZE, ID);
        Seq_free(&myTestSeq);    
}
