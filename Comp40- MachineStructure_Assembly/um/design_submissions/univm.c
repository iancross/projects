#include <seq.h>
#include "univm.h"

#define T UnivM_T

struct UnivM_T{
        Seq_T segments;
        Seq_T freed_mem;
};

extern T UnivM_new(FILE* program){
        (void) program;
        return NULL;
}

extern void UnivM_handleInst(T program, uint32_t inst, int* regs){
        (void) program;
        (void) inst;
        (void) regs;
        return;
}
extern void UnivM_free(T program){
        (void) program;

}

#undef T
