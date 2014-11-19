
/*
 * Emmett Moore and Ian Cross
 * segMem.c - Segmented Memory Representation implementation.
 */
#include <stdio.h>
#include "segMem.h"
#include "uarray.h"
#include "mem.h"
#include "assert.h"
#include "string.h"

static segment_data init_segment(uint32_t* regs, int rc_size);

/* 
 * a new segment is created with number of words equal to what is
 * in register rC, each word is initialized to zero, and the new ID is
 * placed in register rB
 */
extern void segMem_map(segment_data* segments, Seq_T freed_mem, uint32_t* regs, 
                       int* tail, int rc_size, int rb_id)
{       
        int static_tail = *tail;

        /* reusage of freed segment identifiers of available */
        if (Seq_length(freed_mem) > 0) {
                int id = *((int*) Seq_remlo(freed_mem));
                segment_data new_segment = init_segment(regs, rc_size);
                segments[id] = new_segment;
                regs[rb_id] = id;
        }
        /* creates a new segment if an old one is not available */
        else {
                /* creates a new segment at the end of the array of segments */
                segment_data new_segment = init_segment(regs, rc_size);
                segments[static_tail] = new_segment;
                regs[rb_id] = (static_tail);
                (*tail)++;
        }

}
/* initializes the segment to hold regs[rc_size] number of words and each
 * word to equal zero. 
 */
static segment_data init_segment(uint32_t* regs, int rc_size)
{
        
        segment_data new_segment = malloc(sizeof(*new_segment));
        assert(new_segment);
        new_segment->segment = malloc(sizeof(uint32_t) * regs[rc_size]);
        /* each word gets 0 */
        for (uint32_t i = 0; i < regs[rc_size]; i++) {
                new_segment->segment[i] = 0;
        }
        new_segment->segment_size = regs[rc_size];
        return new_segment;
}

/* 
 * an existing segment of the given ID is unmapped, and the id of the
 * unmapped segment is stored for later use.
 */
extern void segMem_unmap(segment_data* segments, Seq_T freed_mem, 
                uint32_t* regs, int rc_id){
        segment_data toFree_struct = segments[regs[rc_id]];
        segments[regs[rc_id]]=NULL;
        free(toFree_struct->segment);
        free(toFree_struct);
        uint32_t* curr_id = malloc(sizeof(uint32_t));
        *curr_id = regs[rc_id];
        Seq_addhi(freed_mem, curr_id);
}

/*
 * the value at rc_index in the #rb_id segment is stored in reg[ra_load]
extern void segMem_load(Seq_T segments, uint32_t* regs, int ra_load, 
                int rb_id, int rc_index){
        segment_data load_segment = (segment_data) Seq_get(segments, 
                                                           regs[rb_id]);
        regs[ra_load] = load_segment->segment[regs[rc_index]];
}
 */ 

/* the value in reg rc is stored in # ra_id segment at rb_index
extern void segMem_store(Seq_T segments, uint32_t* regs, int ra_id, 
                         int rb_index, int rc_store){

        segment_data load_segment = (segment_data) Seq_get(segments, 
                                                           regs[ra_id]);
        load_segment->segment[regs[rb_index]] = regs[rc_store];

} */
