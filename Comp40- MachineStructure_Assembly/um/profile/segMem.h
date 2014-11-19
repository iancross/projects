/*
 * Emmett Moore and Ian Cross
 * segMem.h - Segmented Memory Represenation interface.
 */
#include <stdint.h>
#include <stdlib.h>
#ifndef SEGMEM_INCLUDED
#define SEGMEM_INCLUDED
#include <seq.h>

/* Invariant:
What will be true within this interface is the following:
    The segments in the sequence are either mapped, unmapped, loaded,
    or stored.
    The invariants in UnivM_T are upheld during execution.
    Since there is no data stored in this interface, the class invariant
    is based heavily on the invariant of UnivM_T.
 */

typedef struct segment_data{
       uint32_t* segment;
       int segment_size;
} *segment_data;

/* 
 * a new segment is created with number of words equal to what is
 * in register rC, each word is initialized to zero, and the new ID is
 * placed in register rB
 */
extern void segMem_map(segment_data* segments, uint32_t* freed_mem, 
                       uint32_t* regs, uint32_t* seg_tail, 
                       uint32_t* freed_tail, uint32_t* num_segments, 
                       int rc_size, int rb_id);
/*
 * an existing segment of the given ID is unmapped, 
 * and the id of the unmapped segment is stored for later use.
 */
extern void segMem_unmap(segment_data* segments, uint32_t* freed_mem,
                         uint32_t* freed_tail, uint32_t* regs, 
                         int rc_id);

/*
 * the value at rc_index in the #rb_id segment is stored in reg[ra_load]
 */ 
//extern void segMem_load(Seq_T segments, uint32_t* regs, int ra_load,
//                        int rb_id, int rc_index);

/* the value in reg rc is stored in # ra_id segment at rb_index */
//extern void segMem_store(Seq_T segments, uint32_t* regs, int ra_id, 
//                         int rb_index, int rc_store);

#endif
