/*
 *     print.h
 *     BY Luca Conetta 
 *     Comp40 HW4 -  arith
 *
 *     This program provides the interface for the functions that are used to 
 *     print and read codewords to and from files.
 *
 *     Last Updated: 03/22/2021
 */

#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include <math.h>
#include "bitpack.h"
#include "arith40.h"
#include <mem.h>

/* print_codewords
    Purpose: Prints a header, and then calls a map function that prints each
    codeword in a UArray2 of codewords to stdout. The codewords are accessed as
    uint64_t variables, but are cast to uint32_ts and are then printed 8 bytes 
    at a time using putchar.
    Parameters: A2Methods_UArray2, A2Methods_mapfun, A2Methods_T
    Returns: None.
    Notes: function uses function apply_print_codewords as an apply function.
*/
extern void print_codewords(A2Methods_UArray2 codewords, A2Methods_mapfun *map, 
                                                A2Methods_T methods);
/* apply_print_codewords
    Purpose: Apply function that transforms a codeword representing a 2x2
            block of CV pixels from a uint64_t to a uint32_t, then prints 
            the codeword  8 bytes at a time using putchar.
    Parameters: col and row integers, A2Methods_UArray2, void *elem, void *cl 
    Returns: None.
*/
extern void apply_print_codewords(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl);

/* print_codewords
    Purpose: Reads header of file and stores width and height, then reads 
            each codeword representing a block of CV pixels from a file, 
            then casts the codeword from a uint32_t to a uint64_t, then 
            stores the codeword in a UArray2 of codewords which is returned.
    Parameters: A2Methods_UArray2, A2Methods_mapfun, A2Methods_T
    Returns: A2Methods_UArray2 containing all codewords
    Notes: function uses function apply_print_codewords as an apply function.
*/
extern A2Methods_UArray2 get_codewords(FILE *fp, A2Methods_mapfun *map, 
                                                A2Methods_T methods);

/* apply_get_codewords
    Purpose: Apply function that reads a codeword representing a block of CV 
            pixels from a file, then casts the codeword from a uint32_t to a
            uint64_t, then stores the codeword in a UArray2 of codewords.
    Parameters: col and row integers, A2Methods_UArray2, void *elem, void *cl 
    Returns: None.
*/
extern void apply_get_codewords(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl);

