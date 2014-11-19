/*
 *      main.c
 *      Ian Cross and Emmett Moore
 */

#include <stdio.h>
#include "univm.h"

int main(int argc, char** argv){
        UnivM_T machine = NULL;
        if (argc == 1) {
                machine = UnivM_new(stdin);
                UnivM_runProgram(machine);
        }
        else if (argc == 2) {
                FILE* program = fopen(argv[1], "rb");
                if (program == NULL) {
                        fprintf(stderr, "usage: file does not exist\n");
                        return 0;
                }
                machine = UnivM_new(program);
                //handle instructions 
                fclose(program);
                UnivM_runProgram(machine);
        }
        else if (argc > 2) {
                fprintf(stderr, "usage: too many arguments\n");
                return 0;
        }
        UnivM_free(machine);
}
