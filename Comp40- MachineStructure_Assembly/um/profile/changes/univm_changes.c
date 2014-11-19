/*
 * Ian Cross and Emmett Moore
 * univm.c - Universal Machine implementation
 */
#include <seq.h>
#include "univm.h"
#include "segMem.h"
#include "assert.h"
#include "um-dis.h"
#include "bitpack.h"

#define NUM_REGS 8
#define SEG0_ID 0
#define SEG0_INDEX 0
#define SIZE_INDEX 1
#define PROG_INDEX 2
#define STORE_INDEX 3
#define WORD_SIZE 64
#define HINT 10
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
        Seq_T freed_mem;
        uint32_t regs[NUM_REGS];
        uint32_t prog_counter;
        uint32_t* array_size;
        int* tail;          // the next open free id
};

/*function declarations*/
static UnivM_T init_UM();
static uint32_t get_inst(FILE* program);
static void load_seg0(UnivM_T machine, FILE* program);
static void handle_inst(UnivM_T machine, uint32_t curr_inst);
static void cond_move(uint32_t* regs, int ra, int rb, int rc); 
static void addition(uint32_t* regs, int ra, int rb, int rc); 
static void multiplication(uint32_t* regs, int ra, int rb, int rc); 
static void division(uint32_t* regs, int ra, int rb, int rc); 
static void bitwise_NAND(uint32_t* regs, int ra, int rb, int rc); 
static void halt(UnivM_T machine); 
static void output(uint32_t* regs, int rc); 
static void input(uint32_t* regs, int rc); 
static void load_program(UnivM_T machine, uint32_t* regs, int rb, int rc); 
static void load_value(uint32_t* regs, uint32_t curr_inst); 

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
                   machine->tail, SIZE_INDEX, SEG0_INDEX);
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
        //uint32_t* segment = seg_0->segment;
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
        /* free the sequence of segments */
        int tail = *machine->tail;
        for (int i = 0; i < tail; i++) {
                segment_data curr_segment = machine->segments[i];
  /*              segment_data curr_segment = (segment_data) 
                        Seq_remlo(machine->segments); */
                if (curr_segment != NULL) {
                        free(curr_segment->segment);
                        free(curr_segment);
                }
        }
        free(machine->tail);
        free(machine->array_size);
        free(machine->segments);
        
        
        uint32_t* curr_id;
        int freed_mem_length = Seq_length(machine->freed_mem);
        for (int i = 0; i < freed_mem_length; i++) {
                curr_id = (uint32_t*) Seq_remlo(machine->freed_mem);
                free(curr_id);
        }
        Seq_free(&machine->freed_mem);
        free(machine);
        
}

/* loads the program into segment 0 */
static void load_seg0(UnivM_T machine, FILE* program){
        segment_data seg_0 = machine->segments[0];
        int num_instructions = seg_0->segment_size;
        uint32_t* segment = seg_0->segment; 
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
        Seq_T freed_mem = machine->freed_mem;
       
        uint32_t opcode = (curr_inst >> (32 - OPCODE_WIDTH));
        uint32_t ra = ((curr_inst << 23) >> 29);
        uint32_t rb = ((curr_inst << 26) >> 29);
        uint32_t rc = ((curr_inst << 29) >> 29);
        segment_data load_segment;
        if (opcode == 0){

                cond_move(regs, ra, rb, rc);
        }
        else if (opcode == 1){
                load_segment = segments[regs[rb]];
/*                load_segment = (segment_data) Seq_get(segments, 
                                regs[rb]); */
                regs[ra] = load_segment->segment[regs[rc]];
        }
        else if (opcode == 2){ 
                load_segment = segments[regs[ra]];
                load_segment->segment[regs[rb]] = regs[rc];
        }
        else if (opcode == 3) {addition(regs, ra, rb, rc);}
        else if (opcode == 4) {multiplication(regs, ra, rb, rc);}
        else if (opcode == 5) {division(regs, ra, rb, rc);}
        else if (opcode == 6){ bitwise_NAND(regs, ra, rb, rc); }
        else if (opcode == 7){ halt(machine); }
        else if (opcode == 8){ segMem_map(segments, freed_mem, regs, 
                                          machine->tail,  rc, rb);}
        
        else if (opcode == 9){ segMem_unmap(segments, freed_mem, 
                        regs, rc);}
        else if (opcode == 10){ output(regs, rc);} 
        else if (opcode == 11){ input(regs, rc);}
        else if (opcode == 12){ load_program(machine, regs, rb, rc);}
        else if (opcode == 13){ load_value(regs, curr_inst);}
        else { return; }
}

/* conditionally move a value in rb into rc, depending on value of ra */
static void cond_move(uint32_t* regs, int ra, int rb, int rc){

        if (regs[rc] !=0) {
                regs[ra] = regs[rb];
        }
}

/* add values in rb and rc and put them in ra */
static void addition(uint32_t* regs, int ra, int rb, int rc){
        regs[ra] = ((regs[rb] + regs[rc]) );
}

/* multiply values in rb and rc and put them in ra */
static void multiplication(uint32_t* regs, int ra, int rb, int rc){
        regs[ra] = ((regs[rb] * regs[rc]));
}

/* divide value in rb by that in rc and put it in ra */
static void division(uint32_t* regs, int ra, int rb, int rc){
        regs[ra] = regs[rb] / regs[rc];
} 

/* ra gets bitwise nand of rb and rc */
static void bitwise_NAND(uint32_t* regs, int ra, int rb, int rc){

        regs[ra] = ~(regs[rb] & regs[rc]);
} 

/* frees memory and stops computation */
static void halt(UnivM_T machine){
        (void) machine;
        UnivM_free(machine);
        exit(0);

} 

/* display value in rc immediately (only values from 0 to 255) */
static void output(uint32_t* regs, int rc){
        assert(regs[rc] < 256);
        char toPrint = (char) regs[rc];
        putc(toPrint, stdout);
} 

/* the UM waits for input , and when it arrives it is put into rc */
static void input(uint32_t* regs, int rc){
        int toStore = fgetc(stdin);
        assert(toStore < 256 && toStore >=0);
        if (toStore == 4) {
                regs[rc] = ~0;
        }
        else { regs[rc] = (uint32_t) toStore; }
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
                seg0->segment = malloc(sizeof(uint32_t*) * size);
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

/* loads a value into the register specified in curr_inst  */
static void load_value(uint32_t* regs, uint32_t curr_inst)
{
        uint32_t ra = ((curr_inst << 4) >> 29);
        uint32_t value = ((curr_inst << 7) >> 7);
        regs[ra] = value;
}

/* initializes the UM */
static UnivM_T init_UM(){
        UnivM_T machine = malloc(sizeof(*machine));
        machine->array_size = malloc(sizeof(*machine->array_size));
        *(machine->array_size) = 400000000;
        machine->tail = malloc(sizeof(*machine->tail));
        *(machine->tail) = 0;
        machine->segments = malloc(sizeof(segment_data) * 
                                   (*machine->array_size)); //////
        machine->freed_mem = Seq_new(HINT);
        for (int i = 0; i < NUM_REGS; i++) {
                machine->regs[i] = 0;
        }
        machine->prog_counter = 0;
        return machine;
}


/* gets the next 32 bit instruction of the program */
static uint32_t get_inst(FILE* program){
        uint64_t curr_bits = 0;
        uint32_t curr_word = 0;
        int width = 8;
        int lsb = 24;
        for(int i = 0; i < 4; i++){
                curr_bits = getc(program); /* gets most significant
                                            * byte of program */
                curr_bits <<= (WORD_SIZE - width);
                curr_bits >>= (WORD_SIZE - width);
                curr_word = Bitpack_newu(curr_word, 
                                width, lsb, curr_bits);
                lsb -= 8;
        }
        return curr_word;
}
#undef T
