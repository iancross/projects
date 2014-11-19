//segMem.c
#include "segMem.h"

extern void segMem_map(Seq_T segments, uint32_t size, uint32_t id){
        (void) segments;
        (void) size;
        (void) id;
}
extern void segMem_unmap(Seq_T segments, uint32_t id){
        (void) segments;
        (void) id; 
}
extern uint32_t segMem_load(Seq_T segments, uint32_t id, uint32_t index){
        (void) segments;
        (void) id;
        (void) index;
        return 0;
}
extern void segMem_store(Seq_T segments, uint32_t id, 
                        uint32_t index, uint32_t value){
        (void) segments;
        (void) id;
        (void) index;
        (void) value;
}
