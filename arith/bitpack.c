/*
 *     bitpack.c
 *     BY Luca Conetta
 *     Comp40 HW4 -  arith
 *
 *     This program provides the functions required to make 
 *     Last Updated: 03/22/2021
 */
#include <assert.h>
#include "bitpack.h"
#include <stdio.h>
#include <stdlib.h>

#include "except.h"
#include <stdint.h>

Except_T Bitpack_Overflow = { "Overflow packing bits" };

#define MAX_WIDTH 64

/* fitsu
    Purpose: Determines weather an unsigned integer can be
             represented within a select number of bits (width)
 Parameters: uint64_t of the number of enquiry, 
             unsigned int of the width of bits
    Returns: Boolean true if bits can be represented, false otherwise
*/
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert(width <= MAX_WIDTH);

    uint64_t temp = n;
    temp = temp << (MAX_WIDTH - width);
    temp = temp >> (MAX_WIDTH - width);
    return (temp == n);
}
/* Bitpack_fitss
    Purpose: Determines weather a signed integer can be
             represented within a select number of bits (width)
 Parameters: uint64_t of the number of enquiry, 
             unsigned int of the width of bits
    Returns: Boolean true if bits can be represented, false otherwise
*/
bool Bitpack_fitss( int64_t n, unsigned width)
{
    assert(width <= MAX_WIDTH);
    
    int64_t temp = n;
    temp = temp << (MAX_WIDTH - width);
    temp = temp >> (MAX_WIDTH - width);
    return (temp == n);

}

/* Bitpack_gets
    Purpose: gets an unsigned value from a word given width and starting lsb
    Parameters: uint64_t word, unsigned width, unsigned lsb
    Returns: uint64_t value
*/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= MAX_WIDTH);
    assert((width + lsb) <= MAX_WIDTH);

    if (width == 0){
        return 0;
    }

    word = word << (MAX_WIDTH - (width + lsb));
    word = word >> (MAX_WIDTH - width);

    return word;

}


/* Bitpack_gets
    Purpose: gets a signed value from a word with given width and starting lsb
    Parameters: uint64_t word, unsigned width, unsigned lsb
    Returns: int64_t value
*/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= MAX_WIDTH);
    assert((width + lsb) <= MAX_WIDTH);

    if (width == 0){
        return 0;
    }

    uint64_t u_i = Bitpack_getu(word, width, lsb);
    uint64_t upper = (1 << (width - 1)) - 1;

    if(u_i < upper){
        return (int64_t)u_i;
    }else{
        return (int64_t)(u_i - (1 << (width)));
    }
}

/* Bitpack_newu
    Purpose: return a new word with the bitfield specified by width / lsb 
            updated to contain an unsigned value
    Parameters: uint64_t word, unsigned width, unsigned lsb, int64_t value
    Returns: uint64_t containing new value
*/
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                                                    uint64_t value)
{
   assert(width <= MAX_WIDTH);
   assert((width + lsb) <= MAX_WIDTH);

   if (Bitpack_fitsu(value, width) != true){
        RAISE(Bitpack_Overflow);
        exit(EXIT_FAILURE);
   }
    uint64_t mask = (1 << (MAX_WIDTH - (width + lsb))) - 1;

    mask = mask << (width + lsb);

    uint64_t mask2 = (1 << lsb) - 1;

    mask = mask + mask2;

    if(width != 0){
         return (word & mask) | value << lsb;
    }else{
        return (word & mask);
    } 
}
/* Bitpack_news
    Purpose: return a new word with the bitfield specified by width / lsb 
            updated to contain an signed value
    Parameters: uint64_t word, unsigned width, unsigned lsb, int64_t value
    Returns: uint64_t containing new value
*/
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                                                    int64_t value)
{
   assert(width <= MAX_WIDTH);
   assert((width + lsb) <= MAX_WIDTH);

   if (Bitpack_fitss(value, width) != true){
        RAISE(Bitpack_Overflow);
        exit (EXIT_FAILURE);
   }

    uint64_t mask = ~(0);
    mask = mask << (MAX_WIDTH - (width + lsb));
    mask = mask >> (MAX_WIDTH - width);
    mask = mask << lsb;
    word = word & (~mask);

    uint64_t temp = (uint64_t) value;
    temp = value << (MAX_WIDTH-width);
    temp = temp >> (MAX_WIDTH-(width+lsb));
    return(word | temp);
}