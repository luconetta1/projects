/*
 *     print.c
 *     BY Luca Conetta 
 *     Comp40 HW4 -  arith
 *
 *     This program provides the implementation of the functions that are used
 *     to print and read codewords to and from files.
 *
 *     Last Updated: 03/22/2021
 */

#include "print.h"

void print_codewords(A2Methods_UArray2 codewords, A2Methods_mapfun *map, 
                                                A2Methods_T methods)
{
    int width = methods->width(codewords);
    int height = methods->height(codewords);
    int width2 = 2 * width;
    int height2 = 2 * height;

    
    printf("COMP40 Compressed image format 2\n%u %u\n", width2, height2);

    map(codewords, apply_print_codewords, NULL);
    
}


void apply_print_codewords(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl)
{
    (void)col;
    (void)row;
    (void)array;
    (void)cl;

    uint64_t *int64 = elem;
    uint32_t int32 = *int64;


    for (int i = 24; i >= 0; i -= 8) {
           /* Putchar puts codeword in file*/ 
               putchar(Bitpack_getu(int32, 8, i));
    }

}

A2Methods_UArray2 get_codewords(FILE *fp, A2Methods_mapfun *map, 
                                                A2Methods_T methods)
{
    unsigned width, height;

    int read = fscanf(fp, "COMP40 Compressed image format 2\n%u %u", 
                                                        &width, &height);
    assert(read == 2);
    int c = getc(fp);
    assert(c == '\n');
    
    width  =  width / 2;
    height =  height / 2;

    A2Methods_UArray2 codewords = methods->new(width, height, sizeof(uint64_t));

    map(codewords, apply_get_codewords, fp);

    return codewords;

}

void apply_get_codewords(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl)
{
    (void)col;
    (void)row;
    (void)array;

    FILE *fp = cl;
    assert(fp);

    uint64_t *codeword = elem;

    uint64_t temp = 0;
    uint64_t byte;

    for(int i = 24; i >= 0; i-=8){
        byte = getc(fp);
        byte = byte << i;
        temp = temp | byte;
    }
    //  fprintf(stderr, "Temp: %lu", temp);
    *codeword = temp;
}