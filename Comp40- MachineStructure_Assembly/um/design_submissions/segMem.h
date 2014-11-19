/*
 *       segMem.h - Segment Memory interface
 */
#include <stdint.h>

#ifndef SEGMEM_INCLUDED
#define SEGMEM_INCLUDED
#include <seq.h>

extern void segMem_map(Seq_T segments, uint32_t size, uint32_t id);
extern void segMem_unmap(Seq_T segments, uint32_t id);
extern uint32_t segMem_load(Seq_T segments, uint32_t identifier, 
                            uint32_t index);
extern void segMem_store(Seq_T segments, uint32_t id, 
                        uint32_t index, uint32_t value); 

#endif
