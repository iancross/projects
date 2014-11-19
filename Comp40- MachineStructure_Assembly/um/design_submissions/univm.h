/*
 *      Ian Cross and Emmett Moore
 *      univm.h - Universal Machine Interface
 */
#ifndef UNIVM_INCLUDED
#define UNIVM_INCLUDED
#define T UnivM_T
typedef struct T *T;
#include <stdint.h>
#include <stdio.h>
/*
 * creates a new UnivM struct that contains the sequence of segments and the
 * array of freed memory locations for reuse.
 */
extern T UnivM_new(FILE* program);
/*
 * takes in the UnivM struct, the instruction, and the register array and
 * carries out the specified instruction.
 */
extern void UnivM_handleInst(T program, uint32_t inst, int* regs);
/*
 * frees all of the data in the struct.
 */ 
extern void UnivM_free(T program);
#undef T
#endif
