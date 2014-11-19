/*
 * Emmett Moore and Ian Cross
 * segMem.c - Segmented Memory Representation implementation.
 */
#include <stdio.h>
#include "segMem.h"
#include "uarray.h"
#include "mem.h"
#include "assert.h"

static segment_data init_segment(uint32_t* regs, int rc_size);

/* 
 * a new segment is created with number of words equal to what is
 * in register rC, each word is initialized to zero, and the new ID is
 * placed in register rB
 */
extern void segMem_map(segment_data* segments, uint32_t* freed_mem,
                       uint32_t* regs, uint32_t* seg_tail, 
                       uint32_t* freed_tail, uint32_t* num_segments, 
                       int rc_size, int rb_id)

{       segment_data new_segment = NULL; 
        /* reusage of freed segment identifiers of available */

        if (*freed_tail > 0) {
                int index = *(freed_tail) - 1;
                new_segment = init_segment(regs, rc_size);
                regs[rb_id] = freed_mem[index];
                segments[regs[rb_id]] = new_segment;
                *freed_tail -= 1;
        }
        /* creates a new segment if an old one is not available */
        else {
                uint32_t id = *(seg_tail);
                new_segment = init_segment(regs, rc_size);
                assert(new_segment);
                assert(segments);
                segments[id] = new_segment;
                assert(segments[0]);
                regs[rb_id] = id;
                *(seg_tail) = id  + 1;
                /* resizing */
                if ((*seg_tail) >= (*num_segments)) {
                       segment_data* new_segments = realloc(segments, 
                                                    2*sizeof(segment_data)
                                                    *(*num_segments));
                       if (new_segments != NULL) {
                               segments = new_segments;
                               *(num_segments) = *(num_segments) * 2;
                       }
                }
        }
}
/* initializes the segment to hold regs[rc_size] number of words and each
 * word to equal zero. 
 */
static segment_data init_segment(uint32_t* regs, int rc_size)
{
        uint32_t size = regs[rc_size];
        segment_data new_segment = malloc(sizeof(*new_segment));
        new_segment->segment = malloc(sizeof(uint32_t) * size);
        
        /* each word gets 0 */
        for (uint32_t i = 0; i < size; i++) {
                new_segment->segment[i] = 0;
        }
        new_segment->segment_size = size;
        return new_segment;
}

/* 
 * an existing segment of the given ID is unmapped, and the id of the
 * unmapped segment is stored for later use.
 */
extern void segMem_unmap(segment_data* segments, uint32_t* freed_mem,
                         uint32_t* freed_tail, uint32_t* regs, 
                         int rc_id)
{
        segment_data toFree_struct = segments[regs[rc_id]];
        free(toFree_struct->segment);
        free(toFree_struct);
        freed_mem[(*freed_tail)] = regs[rc_id];
        segments[regs[rc_id]] = NULL;
        *freed_tail += 1;
        
        
        /*uint32_t* curr_id = malloc(sizeof(uint32_t));
        *curr_id = regs[rc_id];
        Seq_addhi(freed_mem, curr_id);
        segments[regs[rc_id]] = NULL;*/
}
