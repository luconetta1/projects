/*
 *     compress40.c
 *     BY Luca Conetta 
 *     Comp40 HW4 -  arith
 *
 *     This program provides the implementation of the compress40 and 
 *     decompress40 functions. 
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
#include "trans.h"
#include "print.h"


/* compress40
    Purpose: Compresses a PPM image so that it is represented by a file
            containing codewords that represent 2x2 blocks of pixels
    Parameters: FILE * containing the image
    Returns: none.
*/
void compress40 (FILE *input){
     
        A2Methods_T methods = uarray2_methods_plain;
        assert(methods);
        A2Methods_mapfun *map = methods->map_default;
        assert(map);
        Pnm_ppm image = Pnm_ppmread(input, methods);

        A2Methods_UArray2 cv_array = rgb_to_cv(image, map, methods);
        
        A2Methods_UArray2 compressed_cv = cv_compress(cv_array, map, methods);

        A2Methods_UArray2 codewords = block_to_codeword(compressed_cv, map, methods);

        print_codewords(codewords, map, methods);

        methods->free(&cv_array);
        methods->free(&compressed_cv);
        methods->free(&codewords);
        Pnm_ppmfree(&image);
}



/* decompress40
    Purpose: Reads a file containing a compressed representation of a Pnm_PPM
            and prints an approximation of the original image to stdout
    Parameters: FILE * containing the compressed image
    Returns: none.
*/
void decompress40 (FILE *input){
        A2Methods_T methods = uarray2_methods_plain;
        assert(methods);
        A2Methods_mapfun *map = methods->map_default;
        assert(map);

        A2Methods_UArray2 codewords = get_codewords(input, map, methods);        

        A2Methods_UArray2 decoded_words = codeword_to_block(codewords, map, methods);

        A2Methods_UArray2 decompressed_cv = decompress_cv(decoded_words, map, methods);

        Pnm_ppm pixmap = cv_to_rgb(decompressed_cv, map, methods);

        Pnm_ppmwrite(stdout, pixmap);

        methods->free(&decompressed_cv);
        methods->free(&decoded_words);
        methods->free(&codewords);
        Pnm_ppmfree(&pixmap);
}