#include <stdio.h>
#include "segMem.h"

#define SIZE 12
#define HINT 15
#define ID 13
#define INDEX 10
#define VALUE 128

int main() {

        Seq_T myTestSeq = Seq_new(HINT);
        segMem_map(myTestSeq, SIZE, ID);
        segMem_store(myTestSeq, ID, INDEX, VALUE);
        
        segMem_unmap(myTestSeq, ID);
        Seq_free(&myTestSeq);    
}       
