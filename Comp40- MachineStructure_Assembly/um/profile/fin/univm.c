/*
 * Ian Cross and Emmett Moore
 * univm.c - Universal Machine implementation
 */
#include <seq.h>
#include "univm.h"
#include "segMem.h"
#include "assert.h"

#define NUM_REGS 8
#define SEG0_ID 0
#define SEG0_INDEX 0
#define SIZE_INDEX 1
#define PROG_INDEX 2
#define STORE_INDEX 3
#define WORD_SIZE 64
#define HINT 100000000
#define OPCODE_WIDTH 4
#define OPCODE_LSB 28
#define REG_WIDTH 3
#define RA_LSB 6
#define RB_LSB 3
#define RC_LSB 0
#define RA_13_LSB 25
#define VALUE_WIDTH 25
#define VALUE_LSB 0


struct UnivM_T{
        segment_data* segments;
        uint32_t* freed_mem;
        uint32_t regs[NUM_REGS];
        uint32_t prog_counter;
        uint32_t* seg_tail;
        uint32_t* num_segments;
        uint32_t* freed_tail;
};

/*function declarations*/
static UnivM_T init_UM();
static uint32_t get_inst(FILE* program);
static void load_seg0(UnivM_T machine, FILE* program);
static void handle_inst(UnivM_T machine, uint32_t curr_inst);
static void load_program(UnivM_T machine, uint32_t* regs, int rb, int rc); 

/* Takes in the program FILE*, finds the size of the program, and 
 * and initializes segment 0.
 */
extern UnivM_T UnivM_new(FILE* program){
        fseek(program, 0L, SEEK_END);
        int num_bytes = ftell(program);
        fseek(program, 0L, SEEK_SET);
        int num_words = num_bytes/4;
        assert(num_words > 0);
        UnivM_T machine = init_UM();
        /*Here we are temporarily assigning the num_words to a register and
          passing it in the segMem_map. This is done so that we can reuse
          the segMem interface to map the program in segment 0. This 
          temporarily breaks the invariant that only the 32bit instructions
          can manipulate the registers.*/
        machine->regs[SEG0_INDEX] = SEG0_ID;
        machine->regs[SIZE_INDEX] = num_words;
        segMem_map(machine->segments, machine->freed_mem, machine->regs,
                        machine->seg_tail, machine->freed_tail, 
                        machine->num_segments, SIZE_INDEX, SEG0_INDEX);
        load_seg0(machine, program);
        /* initializes the registers to equal zero */
        for (int i = 0; i < NUM_REGS; i++) {
                machine->regs[i] = 0;
        }
        return machine;
}
/* runs the program that was loaded into the machine */
extern void UnivM_runProgram(UnivM_T machine)
{
        segment_data seg_0 = machine->segments[0];
        uint32_t curr_inst;
        /* loop invariant: it will run each instruction until it reaches
         * a halt. if no halt is reached, it will go out of bounds 
         * and seg fault as an unchecked runtime error 
         */
        while(1) {
                curr_inst = seg_0->segment[machine->prog_counter];
                handle_inst(machine, curr_inst);
                (machine->prog_counter)++;
        }
}

/* frees the memory inside the UM */
extern void UnivM_free(UnivM_T machine){
        segment_data curr_segment = NULL;
        /* free the sequence of segments */
        uint32_t seq_length = *((uint32_t*)machine->seg_tail);
        for (uint32_t i = 0; i < seq_length; i++) {
                curr_segment = (segment_data) 
                                            machine->segments[i];
                if (curr_segment != NULL) {
                        free(curr_segment->segment);
                        free(curr_segment);
                }
        }
        free(machine->freed_tail);
        free(machine->freed_mem);
        free(machine->segments);
        free(machine->seg_tail);
        free(machine->num_segments);
        free(machine);
}

/* loads the program into segment 0 */
static void load_seg0(UnivM_T machine, FILE* program){
        segment_data* segments = machine->segments;
        int num_instructions = segments[0]->segment_size;
        uint32_t* segment = segments[0]->segment; 
        uint32_t* regs = machine->regs;
        for (int j = 0; j < num_instructions; j++) {
                uint32_t curr_word = get_inst(program);
                regs[PROG_INDEX] = j;
                regs[STORE_INDEX] = curr_word;
                segment[regs[PROG_INDEX]] = regs[STORE_INDEX];
        }
}

/* handles the instruction found from program counter */
static void handle_inst(UnivM_T machine, uint32_t curr_inst)
{       
        uint32_t* regs = machine->regs;
        segment_data* segments = machine->segments;
        uint32_t* freed_mem = machine->freed_mem;

        uint32_t opcode = (curr_inst >> (32 - OPCODE_WIDTH));
        uint32_t ra = ((curr_inst << 23) >> 29);
        uint32_t rb = ((curr_inst << 26) >> 29);
        uint32_t rc = ((curr_inst << 29) >> 29);
        segment_data load_segment;
        if (opcode == 0){
                if (regs[rc] !=0) {
                        regs[ra] = regs[rb];
                }
        }
        else if (opcode == 1){
                load_segment = segments[regs[rb]];
                regs[ra] = load_segment->segment[regs[rc]];
        }
        else if (opcode == 2){ 
                load_segment = segments[regs[ra]];
                load_segment->segment[regs[rb]] = regs[rc];
        }
        else if (opcode == 3) {
                regs[ra] = ((regs[rb] + regs[rc]) );
        }
        else if (opcode == 4) {
                regs[ra] = ((regs[rb] * regs[rc]));
        }
        else if (opcode == 5) {
                regs[ra] = regs[rb] / regs[rc];
        }
        else if (opcode == 6){
                regs[ra] = ~(regs[rb] & regs[rc]);
        }
        else if (opcode == 7){ 
                UnivM_free(machine);
                exit(0);
        }
        else if (opcode == 8){ segMem_map(segments, freed_mem, regs, 
                        machine->seg_tail, machine->freed_tail, 
                        machine->num_segments, rc, rb);}
        else if (opcode == 9){ segMem_unmap(segments, freed_mem, 
                                        machine->freed_tail, regs, rc);}
        else if (opcode == 10){ 
                char toPrint = (char) regs[rc];
                putc(toPrint, stdout);
        } 
        else if (opcode == 11){
                int toStore = fgetc(stdin);
                if (toStore == 4) {
                        regs[rc] = ~0;
                }
        }
        else if (opcode == 12){ load_program(machine, regs, rb, rc);}
        else if (opcode == 13){
                uint32_t ra = ((curr_inst << 4) >> 29);
                uint32_t value = ((curr_inst << 7) >> 7);
                regs[ra] = value;
        }
        else { return; }
}
/* segment m[r[b]] is duplicated and replaces m[0], which is abandoned.
 * program counter is set to m[0][r[c]], and if val in rb is 0, 
 * this operation is expected to be extremely quick. */
static void load_program(UnivM_T machine, uint32_t* regs, int rb, int rc){
        (void) rb;
        if(regs[rb] != 0){
                segment_data seg_to_copy = machine->segments[regs[rb]];
                segment_data seg0 = machine->segments[SEG0_INDEX];

                int size = seg_to_copy->segment_size;

                free(seg0->segment);
                seg0->segment = malloc(sizeof(uint32_t) * size);
                for(int i = 0; i< size; i++){
                        seg0->segment[i] = seg_to_copy->segment[i];
                }
                seg0->segment_size = size;
        }

        /* regs[rc] is decremented in anticipation of being incremented in 
         * run_program loop.
         */
        machine->prog_counter = regs[rc] - 1;
} 
/* initializes the UM */
static UnivM_T init_UM(){
        UnivM_T machine = malloc(sizeof(*machine));
        machine->segments = malloc(sizeof(segment_data) * HINT);
        machine->freed_mem = malloc(sizeof(uint32_t) * HINT);
        for (int i = 0; i < NUM_REGS; i++) {
                machine->regs[i] = 0;
        }
        machine->prog_counter = 0;
        machine->seg_tail = malloc(sizeof(*machine->seg_tail));
        *(machine->seg_tail) = 0;
        machine->num_segments = malloc(sizeof(uint32_t*));
        *(machine->num_segments) = HINT;
        machine->freed_tail = malloc(sizeof(*machine->freed_tail));
        *(machine->freed_tail) = 0;
        return machine;
}


/* gets the next 32 bit instruction of the program */
static uint32_t get_inst(FILE* program){
        uint64_t curr_bits = 0;
        uint32_t curr_word = 0;
        int width = 8;
        int lsb = 24;
        uint64_t right_shifted_word;
        uint64_t left_shifted_word;
        for(int i = 0; i < 4; i++){
                curr_bits = getc(program); /* gets most significant
                                            * byte of program */
                curr_bits <<= (WORD_SIZE - width);
                curr_bits >>= (WORD_SIZE - width);
                right_shifted_word = curr_word >> (width + lsb) << (width + 
                                                                    lsb);
                left_shifted_word = curr_word << (64 - lsb) >> (64 - lsb);
                curr_bits <<= lsb;
                if(lsb == 0){ curr_word = (right_shifted_word | curr_bits);}
                else {curr_word = (right_shifted_word | left_shifted_word) 
                                                      | curr_bits;}
                lsb -= 8;
        }
        return curr_word;
}
#undef T
