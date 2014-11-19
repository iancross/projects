#include <stdlib.h>
#include <stdio.h>
#include "bitpack.h"
#include "segMem.h"
#include <stdint.h>

#define C_MOVE 0
#define SEG_LOAD 1
#define SEG_STORE 2
#define ADD 3
#define MULT 4
#define DIV 5
#define NAND 6
#define HALT 7
#define MAP 8
#define UNMAP 9
#define OUTPUT 10
#define INPUT 11
#define LOAD_PROG 12
#define LOAD_VALUE 13
#define OPCODE_WIDTH 4
#define OPCODE_LSB 28
#define REG_WIDTH 3
#define RA_LSB 6
#define RB_LSB 3
#define RC_LSB 0
#define RA_13_LSB 25
#define VALUE_WIDTH 25
#define VALUE_LSB 0

static void halt();
static void three_register(uint64_t op, uint64_t ra, uint64_t rb, uint64_t rc);
static void test_map_store_load();
static void map(int rb, int rc);
static void store(int ra, int rb, int rc);
static void load(int ra, int rb, int rc);
static void unmap(int rc);
static void load_program(int rb, int rc);
static void test_input();
static void input(int rc);
static void divide(int ra, int rb, int rc);
static void nand(int ra, int rb, int rc);
static void mult(int ra, int rb, int rc);
static void load_val(uint64_t op, uint64_t val, uint64_t ra);
static void add(int ra, int rb, int rc);
static void output(int ra, int rb, int rc);
static void test_add_and_mult ();
static void test_div_and_nand();
static void test_halt();
static void test_load_and_output();
static void test_conditional_move();
static void cond_move(int ra, int rb, int rc);
static void test_load_program();
static void test_freed_mem();

int main(){
        test_input();
        exit(0);
        test_freed_mem();
        test_load_program();
        test_halt();
        test_add_and_mult();
        test_load_and_output();
        test_div_and_nand();
        test_conditional_move();
        test_map_store_load();
}
static void test_freed_mem(){
        load_val(LOAD_VALUE, 34, 0);
        load_val(LOAD_VALUE, 20, 1);
        load_val(LOAD_VALUE, 12, 2);
        load_val(LOAD_VALUE, 3, 3);
        load_val(LOAD_VALUE, 5, 4);
        map(6, 0);
        map(6, 1);
        map(6, 2);
        map(6, 3);
        map(6, 4);
        unmap(3);
        map(6, 4);
        map(6, 4);
        unmap(3);
        unmap(4);
        map(6, 4);
        map(6, 4);
        halt();        
}

static void test_halt() {
        halt();
        load_val(LOAD_VALUE, 'B', 0);
        output(1, 2, 0);
        load_val(LOAD_VALUE, 'A', 0);
        output(1, 2, 0);
        load_val(LOAD_VALUE, 'D', 0);
        output(1, 2, 0);
        load_val(LOAD_VALUE, '!', 0);
        output(1, 2, 0);
        load_val(LOAD_VALUE, '\n', 0);
        output(1, 2, 0);
        halt();        
}

static void test_load_and_output() {
        load_val(LOAD_VALUE, 'G', 0);
        output(1, 2, 0);
        load_val(LOAD_VALUE, 'O', 0);
        output(1, 2, 0);
        load_val(LOAD_VALUE, 'O', 0);
        output(1, 2, 0);
        load_val(LOAD_VALUE, 'D', 0);
        output(1, 2, 0);
        load_val(LOAD_VALUE, '\n', 0);
        output(1, 2, 0);
        halt();
}

static void test_add_and_mult (){
        load_val(LOAD_VALUE, 52, 6);
        //output(0, 0, 6);
        load_val(LOAD_VALUE, 2, 7);
        add(0, 7, 6);
        output(0, 0, 0);
        mult(1, 7, 6);
        output(0, 0, 1);
        halt();        
}

static void test_div_and_nand(){
        load_val(LOAD_VALUE,10000000,7);
        load_val(LOAD_VALUE, 10, 5);
        load_val(LOAD_VALUE,4096,6);
        nand(0, 6, 6);
        divide(1, 0, 7);
        divide(2, 1, 5);
        output(0, 0, 2);
        //=
        load_val(LOAD_VALUE,13245,6);
        load_val(LOAD_VALUE, 7, 5);
        nand(0, 6, 6);
        divide(1, 0, 7);
        divide(2, 1, 5);
        output(0, 0, 2);
        //!
        load_val(LOAD_VALUE,201304,6);
        load_val(LOAD_VALUE, 13, 5);
        nand(0, 6, 6);
        divide(1, 0, 7);
        divide(2, 1, 5);
        output(0, 0, 2);
        //# 35
        load_val(LOAD_VALUE,1024,6);
        load_val(LOAD_VALUE, 12, 5);
        nand(0, 6, 6);
        divide(1, 0, 7);
        divide(2, 1, 5);
        output(0, 0, 2);
        //G 71
        load_val(LOAD_VALUE,1024,6);
        load_val(LOAD_VALUE, 6, 5);
        nand(0, 6, 6);
        divide(1, 0, 7);
        divide(2, 1, 5);
        output(0, 0, 2);
        //U 85
        load_val(LOAD_VALUE,2024,6);
        load_val(LOAD_VALUE, 5, 5);
        nand(0, 6, 6);
        divide(1, 0, 7);
        divide(2, 1, 5);
        output(0, 0, 2);
        halt();        

}

static void test_conditional_move()
{
        //0 screen for ra
        load_val(LOAD_VALUE, 1, 0);  //rc 1 flag
        load_val(LOAD_VALUE, 0, 6);  //NULL  rb
        load_val(LOAD_VALUE, 66, 7); //B   ra
        cond_move(7, 6, 0);
        output(0, 0, 7);
        load_val(LOAD_VALUE, 65, 7); //A   ra
        cond_move(7, 6, 0);
        output(0, 0, 7);
        load_val(LOAD_VALUE, 68, 7); //D   ra
        cond_move(7, 6, 0);
        output(0, 0, 7);
        //GOOD G: 71 O: 79
        load_val(LOAD_VALUE, 0, 0);  //rc 0 flag
        load_val(LOAD_VALUE, 71, 7); //G   ra
        cond_move(7, 6, 0);
        output(0, 0, 7);
        load_val(LOAD_VALUE, 79, 7); //O   ra
        cond_move(7, 6, 0);
        output(0, 0, 7);
        output(0, 0, 7);
        load_val(LOAD_VALUE, 68, 7); //D   ra
        cond_move(7, 6, 0);
        output(0, 0, 7);
        halt();        
}


static void add(int ra, int rb, int rc){
        three_register(ADD, ra, rb, rc);
}
static void mult(int ra, int rb, int rc){
        three_register(MULT, ra, rb, rc);
}
static void divide(int ra, int rb, int rc){
        three_register(DIV, ra, rb, rc);

}
static void nand(int ra, int rb, int rc){
        three_register(NAND, ra, rb, rc);
}
static void halt(){
        three_register(HALT, 0, 0, 0);
}

static void test_input(){
        input(1);
        input(2);
        input(3);
        input(4);
        output(0, 0, 1);
        output(0, 0, 2);
        output(0, 0, 3);
        output(0, 0, 4);
        halt();        
}
static void input(int rc){
        three_register(INPUT, 0, 0, rc);
}

static void test_map_store_load(){

        load_val(LOAD_VALUE, 26, 7);      //30: # of words to be mapped
        map(0, 7);              // passes in a reg to store id and#26
        load_val(LOAD_VALUE, 1, 5);//seg id
        for (int i=0; i<26; i++) {
                load_val(LOAD_VALUE, i, 6);//seg index
                load_val(LOAD_VALUE, i+65, 7);//seg value
                store(5, 6, 7);       //load reg 7 into segID 1 index 15  
        }
        for (int i=0; i<26; i++) {
                load_val(LOAD_VALUE, i, 6);//seg index
                load(4, 5, 6);
                output(0, 0, 4);
        }
        load_val(LOAD_VALUE, 1, 1);
        unmap(1);
        halt();
}
static void test_load_program(){
        //tests load within seg0--i.e. the program counter is reset
        load_val(LOAD_VALUE, 7, 4);     //$r[4] = 7 -- size of seg1
        load_val(LOAD_VALUE, 1, 3);     //$r[3] = 1 -- index of seg1
        map(3, 4);                      // maps the segment
        load_val(LOAD_VALUE, 0, 2);     //$r[2] = 0 -- index of seg0
        for (int i = 0; i < 7; i++) {  
                load_val(LOAD_VALUE, i, 4);      //index in seg1
                load_val(LOAD_VALUE, 41 + i, 7); // $r[7] = index of inst.
                load(6, 2, 7);                   // $r[6] = m[0][index]
                store(3, 4, 6);                  // $m[1][r[4]] = $r[6]
        }
        load_val(LOAD_VALUE, 41, 1); 
        load_program(2, 1);
        load_val(LOAD_VALUE, 66, 0); //load B 
        output(0, 0, 0);
        load_val(LOAD_VALUE, 65, 0); //load A 
        output(0, 0, 0);
        load_val(LOAD_VALUE, 68, 0); //load D 
        output(0, 0, 0);
        halt();
        load_val(LOAD_VALUE, 71, 0); //load G 
        output(0, 0, 0);
        load_val(LOAD_VALUE, 79, 0); //load O 
        output(0, 0, 0);
        output(0, 0, 0);
        load_val(LOAD_VALUE, 68, 0); //load D
        output(0, 0, 0);
        halt();

}
static void load_program(int rb, int rc){
        three_register(LOAD_PROG, 0, rb, rc);
}

static void map(int rb, int rc){
        three_register(MAP,0, rb, rc);
}
static void unmap(int rc){
        three_register(UNMAP, 0, 0, rc);
}
static void store(int ra, int rb, int rc){
        three_register(SEG_STORE, ra, rb, rc);

}
static void load(int ra, int rb, int rc){
        three_register(SEG_LOAD, ra, rb, rc);

}
static void output(int ra, int rb, int rc){
        three_register(OUTPUT, ra, rb, rc);
}

static void cond_move(int ra, int rb, int rc){
        three_register(C_MOVE, ra, rb, rc);
}
static void three_register(uint64_t op, uint64_t ra, 
                uint64_t rb, uint64_t rc)
{
        uint64_t new_inst = 0;
        new_inst = Bitpack_newu(new_inst, OPCODE_WIDTH, OPCODE_LSB, op);
        new_inst = Bitpack_newu(new_inst, REG_WIDTH, RA_LSB, ra);
        new_inst = Bitpack_newu(new_inst, REG_WIDTH, RB_LSB, rb);
        new_inst = Bitpack_newu(new_inst, REG_WIDTH, RC_LSB, rc);
        uint32_t toPrint = (uint32_t) new_inst;
        for (int lsb = 24; lsb >=0; lsb-=8) {
                putc(Bitpack_getu(toPrint, 8, lsb), stdout);
        }
}

static void load_val(uint64_t op, uint64_t val, uint64_t ra)
{
        uint64_t new_inst = 0;
        new_inst = Bitpack_newu(new_inst, OPCODE_WIDTH, OPCODE_LSB, op);
        new_inst = Bitpack_newu(new_inst, REG_WIDTH, RA_13_LSB, ra);
        new_inst = Bitpack_newu(new_inst, VALUE_WIDTH, VALUE_LSB, val);

        uint32_t toPrint = (uint32_t) new_inst;
        for (int lsb = 24; lsb >=0; lsb-=8) {
                putc(Bitpack_getu(toPrint, 8, lsb), stdout);
        }
}

