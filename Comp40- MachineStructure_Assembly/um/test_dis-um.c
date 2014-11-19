#include <stdio.h>
#include <stdint.h>
#include "um-dis.h"

int main() {
        uint32_t myInt = 268435466;
        const char* myString = Um_disassemble(myInt);
        printf("%s\n", myString);
}

