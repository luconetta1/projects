/*
 *     trans.c
 *     BY Luca Conetta
 *     Comp40 HW4 -  arith
 *
 *     This program provides the transformation functions and their relevant 
 *     apply functions that complete the compression and decompression steps.
 *
 *     Last Updated: 03/22/2021
 */

#include "trans.h"

/* HELPER FUNCTION INTERFACES */

/* ensure_range
      Purpose: Ensures that a rgb value is between -1.0 and 1.0
   Parameters: Float representing rgb value
      Returns: Float rgb adjusted to be within range if need be
*/
float ensure_range(float rgb);
/* round_a
      Purpose: Ensures that the value for a is between 0 and 1.0 and returns
               the equivalent value in uint64_t format
   Parameters: float representing the a value
      Returns: uint64_t of the same a value adjusted for range
*/
uint64_t round_a(float a);
/* round_BCD
      Purpose: Ensures that the value for a BCD is between -0.3 and 0.3 and 
               returns the equivalent value in int64_t format.
   Parameters: float representing the bcd value
      Returns: int64_t of the same value adjusted for range
        Notes: 
*/
int64_t round_bcd(float bcd);


A2Methods_UArray2 rgb_to_cv(Pnm_ppm image, A2Methods_mapfun *map, 
                                                A2Methods_T methods)
{
    int width = image->width;
    int height = image->height;

    /* trimming the height and width if need be*/
    if (width % 2 == 1){
        width -= 1;
    }
    if (height % 2 == 1){
        height -= 1;
    }
    /* Make new array of cv_pixels */
    A2Methods_UArray2 cv_image = methods -> new(width, height, 
                                        sizeof(struct cv_pixel));

    /* Closure for the apply function with methods,
       denominator and array to be written to */
    struct closure *cl = NEW(cl);
    cl -> methods = methods;
    cl -> array = cv_image;
    cl -> denominator = image -> denominator;

    map(image->pixels, apply_rgb_to_cv, cl);

    /* free the closure. */
    if(cl != NULL){
        free(cl);
        cl = NULL;
    }

    return cv_image;
}
                                                                            
void apply_rgb_to_cv(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl)
{
    (void) array;
    /* Cast the struct from the closure */
    struct closure *this_cl = cl;

    /* If we reach to the width/height limit, exit. 
        Fixes segfault problem when we trim the height/width */
    if ((col >= this_cl->methods->width(this_cl->array)) || 
                (row >= this_cl->methods->height(this_cl->array))){
                return;
        }
    /* Current pixel */
    Pnm_rgb rgb_pixel = elem;
        
    struct cv_pixel *ybr_pixel = this_cl->methods->at(this_cl -> 
                                               array, col, row);
           
    float r,g,b;
    /* Compute the y, pb, pr values from the rgb values and 
       set within array */
    r = (rgb_pixel->red * 1.0) / this_cl->denominator;
    g = (rgb_pixel->green * 1.0) / this_cl->denominator;
    b = (rgb_pixel->blue * 1.0) / this_cl->denominator;
    
    ybr_pixel->y = (0.299 * r) + (0.587 * g) + (0.114 * b);
    ybr_pixel->pb = (-0.168736 * r) - (0.331264 * g) + (0.5 * b);
    ybr_pixel->pr =  (0.5 * r) - (0.418688 * g) - (0.081312 * b);

}

Pnm_ppm cv_to_rgb(A2Methods_UArray2 cv_image, A2Methods_mapfun *map, 
                                                A2Methods_T methods)
{
    int width = methods->width(cv_image);
    int height = methods->height(cv_image);

    unsigned denominator = 255;

    /* Declare new pnm_ppm of the final image and set attribute values*/
    Pnm_ppm final_image;
    NEW(final_image);
    final_image->width = width;
    final_image->height = height;
    final_image->denominator = denominator;
    final_image->methods = methods;

    /* New array for the rgb pixels*/
    final_image->pixels = methods->new(width, height, sizeof(struct Pnm_rgb));

    /* Closure for the apply function with methods,
       denominator and array to be written to */
    struct closure *cl = NEW(cl);
    cl -> methods = methods;
    cl -> array = final_image->pixels;
    cl -> denominator = denominator;

    map(cv_image, apply_cv_to_rgb, cl);

    /* free the closure. */
    if(cl != NULL){
        free(cl);
        cl = NULL;
    }
 
    return final_image;

}


void apply_cv_to_rgb(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl)
{
    (void) array;
    /* Cast the struct from the closure */
    struct closure *this_cl = cl;

    struct cv_pixel *cv_pix = elem;
    Pnm_rgb rgb_pixel = this_cl->methods->at(this_cl -> array, col, row);

    float r,g, b;
    /* Compute rgb values from pb, pr, y */

    r = (1.0 * cv_pix->y + 0.0 * cv_pix->pb + 1.402 * cv_pix->pr);
    g = (1.0 * cv_pix->y - 0.344136 * cv_pix->pb - 0.714136 * cv_pix->pr);
    b = (1.0 * cv_pix->y + 1.772 * cv_pix->pb + 0.0 * cv_pix->pr);

    rgb_pixel->red = round(ensure_range(r) * this_cl->denominator);
    rgb_pixel->green = round(ensure_range(g) * this_cl->denominator);
    rgb_pixel->blue = round(ensure_range(b) * this_cl->denominator);

}

A2Methods_UArray2 cv_compress(A2Methods_UArray2 cv_image, 
                                 A2Methods_mapfun *map, A2Methods_T methods)
{
    /* Due to the 2*2 blocks, width and height are adjusted */
    int new_width = (methods->width(cv_image) / 2);
    int new_height = ((methods->height(cv_image) / 2));

    /* Declare new array to store the 2*2 blocks */
    A2Methods_UArray2 compressed = methods -> new(new_width, new_height, 
                                        sizeof(struct compressed_block));
    
    /* Closure for the apply function with methods,
       denominator and array to be written to */
    struct closure *cl = NEW(closure);
    cl->methods = methods;
    cl->array = compressed;
    cl->denominator = 255;

    map(cv_image, apply_cv_compress, cl);

    /* free the closure. */
    if(cl != NULL){
        free(cl);
        cl = NULL;
    }

    return compressed;
}


void apply_cv_compress(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl)
{
    /* Cast the struct from the closure */
    struct closure *this_cl = cl;
    /* Read in the current cv_pixel from elem */
    struct cv_pixel *this_cv = elem;
    struct cv_pixel *pixel1, *pixel2, *pixel3, *pixel4;
    float avg_pb, avg_pr, a, b, c, d;
    
    if (col % 2 == 0 && row % 2 == 0){
        /* condition met at the beginning of each 2*2 block */
        pixel1 = this_cv;
        /* get rest of the pixels within the block */
        pixel2 = this_cl->methods->at(array, col + 1, row);
        pixel3 = this_cl->methods->at(array, col, row + 1);
        pixel4 = this_cl->methods->at(array, col + 1, row + 1);
       
       /* Compute a, b, c, d, avg_pb, avg_pr values from the pixels */
        a = (pixel4->y + pixel3->y + pixel2->y + pixel1->y)/4.0;
        b = (pixel4->y + pixel3->y - pixel2->y - pixel1->y)/4.0;
        c = (pixel4->y - pixel3->y + pixel2->y - pixel1->y)/4.0;
        d = (pixel4->y - pixel3->y - pixel2->y + pixel1->y)/4.0;
        avg_pb = (pixel4->pb + pixel3->pb + pixel2->pb + pixel1->pb)/4.0;
        avg_pr = (pixel4->pr + pixel3->pr + pixel2->pr + pixel1->pr)/4.0;
        
        /* Write the values to the array of blocks */
        struct compressed_block *this_cw = this_cl->methods->at(this_cl->array,
                                                              col / 2, row/ 2);
        
        this_cw->a = round_a(a); 
        this_cw->b = round_bcd(b);
        this_cw->c = round_bcd(c);
        this_cw->d =  round_bcd(d);
        this_cw->pb = (uint64_t)(Arith40_index_of_chroma(avg_pb));
        this_cw->pr = (uint64_t)(Arith40_index_of_chroma(avg_pr));
    }
}


extern A2Methods_UArray2 decompress_cv(A2Methods_UArray2 compressed_image,
                        A2Methods_mapfun *map, A2Methods_T methods)
{
    /* Due to the nature of 2*2 blocks, width and height are adjusted */
    int new_width = (methods->width(compressed_image) * 2);
    int new_height = ((methods->height(compressed_image) * 2));

    /* Create new 2d array to store the CV pixels */
    A2Methods_UArray2 cv_image = methods -> new(new_width, new_height, 
                                        sizeof(struct cv_pixel));

    /* Create the struct closure for the apply function. 
        Struct contains methods, array to be written and denominator */
    struct closure *cl = NEW(closure);
    cl->methods = methods;
    cl->array = cv_image;
    cl->denominator = 255;                            

    map(compressed_image, apply_decompress_cv, cl);
    
    /* free the closure. */
    if(cl != NULL){
        free(cl);
        cl = NULL;
    }

    return cv_image;
}

void apply_decompress_cv(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl)
{
    (void) array;
    /* Cast the struct from the closure */
    struct closure *this_cl = cl;

    /* Read current block, from elem */
    struct compressed_block *this_cw = elem;
    struct cv_pixel *pixel1, *pixel2, *pixel3, *pixel4;
    float avg_pb, avg_pr, a, b, c, d;

    /* Double row and col because of the 2*2 blocks */
    int new_col = col * 2; 
    int new_row = row * 2; 
    
    /* Set the relevant pixel positions within the array corresponding to 
       the 2*2 block */
    pixel1 = this_cl->methods->at(this_cl->array, new_col, new_row);
    pixel2 = this_cl->methods->at(this_cl->array, new_col + 1, new_row);
    pixel3 = this_cl->methods->at(this_cl->array, new_col, new_row + 1);
    pixel4 = this_cl->methods->at(this_cl->array, new_col + 1, new_row + 1);

   /* Compute avg pb, avg pr */
    avg_pb = Arith40_chroma_of_index(this_cw -> pb);
    avg_pr = Arith40_chroma_of_index(this_cw -> pr);
    
    /* Set the pixels' pb, pr to the average */
    pixel1->pb = avg_pb;
    pixel2->pb = avg_pb;
    pixel3->pb = avg_pb;
    pixel4->pb = avg_pb;
    pixel1->pr = avg_pr;
    pixel2->pr = avg_pr;
    pixel3->pr = avg_pr;
    pixel4->pr = avg_pr;
    
    /* Converting from float to int64_t anf uint64_t */
    a = (this_cw -> a * 1.0)/63;
    b = (this_cw -> b * 1.0)/(31 / 0.3);
    c = (this_cw -> c * 1.0)/(31 / 0.3);
    d = (this_cw -> d * 1.0)/(31 / 0.3);

    /* Compute values of y for each pixel */
    pixel1->y = ensure_range(a - b - c + d);
    pixel2->y = ensure_range(a - b + c - d);
    pixel3->y = ensure_range(a + b - c - d);
    pixel4->y = ensure_range(a + b + c + d);
}


A2Methods_UArray2 block_to_codeword(A2Methods_UArray2 compressed_image,
                                 A2Methods_mapfun *map, A2Methods_T methods)
{
    /* Set width and height */
    int width = methods->width(compressed_image);
    int height = methods->height(compressed_image);

    /* Create new 2d array for the codewords */
    A2Methods_UArray2 codewords = methods->new(width, height, 
                                            sizeof(uint64_t));
                                            
    /* Closure for the apply function with methods,
       denominator and array to be written to */
    struct closure *cl = NEW(cl);
    cl -> methods = methods;
    cl -> array = codewords;
    cl -> denominator = 255;

    map(compressed_image, apply_block_to_codeword, cl);

    /* free the closure. */
    if(cl != NULL){
        free(cl);
        cl = NULL;
    }
 
    return codewords;
}

void apply_block_to_codeword(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl)
{
    (void) array;
    struct closure *this_cl = cl;

    struct compressed_block *curr_block = elem;

    /* sets uint64_t final_codeword to point to col, row in  */
    uint64_t *final_codeword = this_cl->methods->at(this_cl->array, col, row);

    uint64_t codeword = 0;

    /* packing data from compressed block struct to codeword */
    codeword = Bitpack_newu(codeword, 4, 0, curr_block->pr);
    codeword = Bitpack_newu(codeword, 4, 4, curr_block->pb);
    codeword = Bitpack_news(codeword, 6, 8, curr_block->d);
    codeword = Bitpack_news(codeword, 6, 14, curr_block->c);
    codeword = Bitpack_news(codeword, 6, 20, curr_block->b);
    codeword = Bitpack_newu(codeword, 6, 26, curr_block->a);


    *final_codeword = codeword;
}


A2Methods_UArray2 codeword_to_block(A2Methods_UArray2 codewords,
                                 A2Methods_mapfun *map, A2Methods_T methods)
{
     /* Set width and height */
    int width = methods->width(codewords);
    int height = methods->height(codewords);
    /* Create new 2d array to store the blocks */
    A2Methods_UArray2 blocks = methods->new(width, height, 
                                sizeof(struct compressed_block));

    /* Closure for the apply function with methods,
       denominator and array to be written to */
    struct closure *cl = NEW(cl);
    cl -> methods = methods;
    cl -> array = blocks;
    cl -> denominator = 255;

    map(codewords, apply_codeword_to_block, cl);
    
    /* free the closure. */
    if(cl != NULL){
        free(cl);
        cl = NULL;
    }
 
    return blocks;
}

void apply_codeword_to_block(int col, int row, A2Methods_UArray2 array, 
                                                void *elem, void *cl)
{
    (void)array;
    /* Cast the struct from the closure */
    struct closure *this_cl = cl;

    /* Read current codeword, from elem */
    uint64_t codeword = *((uint64_t *)elem);

    /* Read the appropriate block from the array */
    struct compressed_block *block = this_cl->methods->at(this_cl->array, 
                                                               col, row);
 
    /* Obtain a, b, c, d, pb, pr values from the codeword */
    block->a = Bitpack_getu(codeword, 6, 26);
    block->b = Bitpack_gets(codeword, 6, 20);
    block->c = Bitpack_gets(codeword, 6, 14);
    block->d = Bitpack_gets(codeword, 6, 8);
    block->pb = Bitpack_getu(codeword, 4, 4);
    block->pr = Bitpack_getu(codeword, 4, 0);

}


float ensure_range(float rgb)
{
    if(rgb < 0){
        return 0.0;
    }
    if(rgb > 1){
        return 1.0;
    }
    return rgb;
}

uint64_t round_a(float a)
{
    if(a < 0){
        a = 0;
    }
    if(a > 1){
        a = 1;
    }

    return (int64_t)(round(a * 63));
}

int64_t round_bcd(float bcd)
{
    if(bcd > 0.3){
        bcd = 0.3;
    }
    if(bcd < -0.3){
        bcd = -0.3;
    }

    return (int64_t)(round(bcd * (31 / 0.3)));   /* 50 = 15/0.3  */
}