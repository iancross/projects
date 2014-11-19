/*
 *      Ian Cross and Emmett Moore
 *      univm.h - Universal Machine Interface
 */

#include <stdint.h>
#include <stdio.h>
#ifndef UNIVM_INCLUDED
#define UNIVM_INCLUDED
#define T UnivM_T

/*
Interface Invariant:
Upon initializing the struct UnivM via UnivM_new(),
Segment 0 contains the program. For each instruction that the UnivM_T 
takes in via UnivM_handleInst(), it executes a valid opcode (0-13).
UnivM_T always first map segments with identifiers in the freed_mem array.
If freed_mem is empty, a new identifier is created to map that segment.
When a new segment is unmapped, its identifier exists until the UnivM 
struct is freed via UnivM_free(). 
*/

typedef struct T *T;

/*
 * creates a new UnivM struct that contains the sequence of segments and the
 * array of freed memory locations for reuse.
 */
extern T UnivM_new(FILE* program);

/*
 * takes in the UnivM struct, the instruction, and the register array and
 * carries out the specified instruction.
 */
extern void UnivM_runProgram(T machine);

 /* frees all of the data in the struct. */
extern void UnivM_free(T program);

#undef T
#endif
