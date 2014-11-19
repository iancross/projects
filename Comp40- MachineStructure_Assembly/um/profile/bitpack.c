/* bitpack.c
 * By Emmett Moore and Dan Defossez
 * 2/24/13
 *
 * Description: Implementation for various bit operations.
 */
#include <stdio.h>
#include "math.h"
#include "except.h"
#include "bitpack.h"
#include "assert.h"

#define SIZE_OF_WORD 64

Except_T Bitpack_Overflow = { "Overflow packing bits" };

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        if (width > 63) {
                return 1;
        }
        if (width < 1) {
                return 0;
        }
        /* For unsigned ints, they will fit in x-bits as long as 
         * they are less than 2 ^ x.
         */
        if (n < ((uint64_t)1 << width)) {
                return 1;
        }
        return 0;

}


bool Bitpack_fitss(int64_t n, unsigned width)
{
        uint64_t invrs_n;
        /* If width is greater than 63, a 64 bit int must be able to fit */
        if (width > 63) {  
                return 1;
        }
        /* If width is less than 1, no integer can fit in that space */
        if (width < 1) { 
                return 0;
        }
        /* If it's a positive int, check if n < 2 ^ (width-1) */
        if (n > 0) {
                if ((uint64_t)n < ((uint64_t)1 << (width-1))) {
                        return 1;
                }
        }
        /* The integer 0 will fit in any non-zero (positive) width */
        else if (n == 0) {
                if (width > 0) {
                        return 1;
                }
        }
        /* If n is less than zero, make it positive, and check if it 
         * is less than OR EQUAL to 2 ^ (width-1)
         */
        else if (n < 0) {
                invrs_n = (uint64_t)(n * (-1));
                if (invrs_n <= ((uint64_t)1 << (width-1))) {
                             return 1;
                }
        }
        return 0;     
}



extern uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (width == 0) {
                return 0;
        }
        /* Set all bits to 0 except those from lsb to lsb + width
         * This isolates the field to return
         */
        word <<= SIZE_OF_WORD - width - lsb;
        word >>= SIZE_OF_WORD - width;  
        return word;
}



int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (width == 64) {
                return (int64_t) word;
        }
        if (width == 0) {
                return 0;
        }
        uint64_t unsign = Bitpack_getu(word, width, lsb);
        /* check msb for positive or negative */
        uint64_t unsign_copy = unsign;
        unsign_copy >>= (width - 1);
        /* if it is a positive number */
        if (unsign_copy == 0) {
                return (int64_t) unsign;
        }
        /*if it is a negative number */
        /* make toReturn all 1's  */
        uint64_t toReturn = 0;
        toReturn = ~toReturn;
        /* combine toReturn values with unsign values */
        toReturn <<= width;
        toReturn |= unsign;
        return (int64_t) toReturn;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value)
{
        if (width == 0) {
                return word;
        }
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        /* get rid of bits past width'th bit */
        value <<= (SIZE_OF_WORD - width);
        value >>= (SIZE_OF_WORD - width);

        uint64_t word_copy = word;
        /* store bits before the lsb */
        word_copy <<= (64 - lsb);
        word_copy >>= (64 - lsb);
        uint64_t bits_b4_lsb = word_copy;
        /* combine new value and bits_b4_lsb */
        value <<= lsb;
        value |= bits_b4_lsb;
        /* combine bits more significant than value in orig word with value */
        word_copy = word;
        word_copy >>= width + lsb;
        word_copy <<= width + lsb;
        value = value | word_copy;
        return value;
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                      int64_t value)
{     //  fprintf(stderr, "S\n");

        if (width == 0) {
                return word;
        }
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        uint64_t value_u = (uint64_t) value;
        /* trim value to a more concise representation */
        value_u <<= (SIZE_OF_WORD - width);
        value_u >>= (SIZE_OF_WORD - width);
        return Bitpack_newu(word, width, lsb, value_u);
}
