/*
 *     trans.h
 *     BY Luca Conetta 
 *     Comp40 HW4 -  arith
 *
 *     This program provides the interface for transformation functions 
 *     and their relevant apply functions that complete the compression 
 *     and decompression steps.
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


struct closure {
    A2Methods_UArray2 *array;
    A2Methods_T methods;
    unsigned denominator;

} *closure;

struct cv_pixel {
    float y;
    float pb;
    float pr;
} *cv_pixel;


struct compressed_block{
    uint64_t a;
    int64_t  b;
    int64_t  c;
    int64_t  d;
    uint64_t pb;
    uint64_t pr;    
} *compressed_block;

/* rgb_to_cv
    Purpose: Transforms a PPM of rgb pixels into a UArray of component-video
            pixels. Takes both a map function and methods suite as input
            in addtion to the PPM
    Parameters: Pnm_pmm, A2Methods_mapfun, A2Methods_T
    Returns: A2Methods_UArray2
    Notes: function uses function apply_rgb_to_cv as an apply function.
*/
extern A2Methods_UArray2 rgb_to_cv(Pnm_ppm image, A2Methods_mapfun *map, 
                                                A2Methods_T methods);

/* apply_rgb_to_cv
    Purpose: Apply function that transforms individual pixels in a PPM
            into CV pixels in a UArray2
    Parameters: col and row integers, A2Methods_UArray2, void *elem, void *cl 
    Returns: None.
*/
extern void apply_rgb_to_cv(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl);

/* cv_to_rgb
    Purpose: Transforms a UArray2 of CV pixels into a PPM image containing
            pixels with rgb values.
    Parameters: PA2Methods_UArray2, A2Methods_mapfun, A2Methods_T
    Returns: Pnm_ppm containing image
    Notes: function uses function apply_cv_to_rgb as an apply function.
*/
extern Pnm_ppm cv_to_rgb(A2Methods_UArray2 cv_image, A2Methods_mapfun *map, 
                                                A2Methods_T methods);

/* apply_cv_to_rgb
    Purpose: Apply function that transforms individual cv pixels in a UArray2
            into rgb pixels in a Pnm_ppm
    Parameters: col and row integers, A2Methods_UArray2, void *elem, void *cl 
    Returns: None.
*/
extern void apply_cv_to_rgb(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl);

/* cv_compress
    Purpose: Transforms a UArray2 of CV pixels into a UArray2 with each 
            cell containing data from a correspoding 2x2 block of the original 
            image.
    Parameters: A2Methods_UArray2, A2Methods_mapfun, A2Methods_T
    Returns: A2Methods_UArray2 containing 2x2 blocks.
    Notes: function uses function apply_cv_compress as an apply function.
*/
extern A2Methods_UArray2 cv_compress(A2Methods_UArray2 cv_image, 
                                 A2Methods_mapfun *map, A2Methods_T methods);

/* apply_cv_compress
    Purpose: Apply function that transforms a 2x2 block of individual cv pixels 
            in a UArray2 into a single struct stored in another UArray2
    Parameters: col and row integers, A2Methods_UArray2, void *elem, void *cl 
    Returns: None.
*/
extern void apply_cv_compress(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl);

/* decompress_cv
    Purpose: Transforms a UArray2 of 2x2 blocks of CV pixels back into a 
            UArray2 where each element is a single CV pixel.
    Parameters: A2Methods_UArray2, A2Methods_mapfun, A2Methods_T
    Returns: A2Methods_UArray2 containing data that represents 2x2 blocks of 
            CV pixels.
    Notes: function uses function apply_decompress_cv as an apply function.
*/
extern A2Methods_UArray2 decompress_cv(A2Methods_UArray2 compressed_image,
                        A2Methods_mapfun *map, A2Methods_T methods);

/* apply_decompress_cv
    Purpose: Apply function that transforms a single struct representing a 2x2
            block of CV pixels in a UArray2 into 4 separate pixels stored in 
            another UArray2
    Parameters: col and row integers, A2Methods_UArray2, void *elem, void *cl 
    Returns: None.
*/
extern void apply_decompress_cv(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl);

/* block_to_codeword
    Purpose: Transforms a UArray2 of 2x2 blocks of CV pixels into a UArray2
            where each element is a uint64_t codeword containing the data 
            from the corresponding element in the input UArray2.s
    Parameters: A2Methods_UArray2, A2Methods_mapfun, A2Methods_T
    Returns: A2Methods_UArray2 containing uint64_t codewords.
    Notes: function uses function apply_block_to_codeword as an apply function.
*/
extern A2Methods_UArray2 block_to_codeword(A2Methods_UArray2 compressed_image,
                                 A2Methods_mapfun *map, A2Methods_T methods);

/* apply_block_to_codeword
    Purpose: Apply function that transforms a single struct representing a 2x2
            block of CV pixels in a UArray2 into a codeword that can be 
            represented in less than 32 digits of a uint64_t
    Parameters: col and row integers, A2Methods_UArray2, void *elem, void *cl 
    Returns: None.
*/
extern void apply_block_to_codeword(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl);

/* block_to_codeword
    Purpose: Transforms a UArray2 of codewords representing a 2x2 block of
            CV pixels into a UArray2 containing structs that hold the data
            that defines those 2x2 blocks
    Parameters: A2Methods_UArray2, A2Methods_mapfun, A2Methods_T
    Returns: A2Methods_UArray2 containing data that represents 2x2 blocks of 
            CV pixels.
    Notes: function uses function apply_codeword_to_block as an apply function.
*/
extern A2Methods_UArray2 codeword_to_block(A2Methods_UArray2 codewords,
                                 A2Methods_mapfun *map, A2Methods_T methods);

/* apply_block_to_codeword
    Purpose: Apply function that transforms a codeword representing a 2x2
            block of CV pixels in a UArray2 into a struct contains the same 
            data.
    Parameters: col and row integers, A2Methods_UArray2, void *elem, void *cl 
    Returns: None.
*/
extern void apply_codeword_to_block(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl);



