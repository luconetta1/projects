/*
 *     ppmdiff.c
 *     BY Luca Conetta 
 *     Comp40 HW4 -  arith
 *
 *     This program calculates the average difference between 2 image files of
 *     ppm format. Program halts with a runtime error if 2 input commads are not
 *     supplied. 
 *
 *     Last Updated: 03/22/2021
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <a2methods.h>
#include <pnm.h>
#include <math.h>
#include <a2plain.h>



int main (int argc, char *argv[]){
    FILE *f1 = NULL;
    FILE *f2 = NULL;

     /* default to UArray2 methods */
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);

    assert(argc == 3);
    /* CRASHES WHEN NO ARGUMENTS) */

    if (argc > 3){
        fprintf(stderr, "Too many arguments");
        exit(EXIT_FAILURE);
    }

    if(argv[1][0] == '-'){
        f1 = stdin;
        f2 = fopen(argv[2], "r");
    }
    else if(argv[2][0] == '-'){
        f2 = stdin;
        f1 = fopen(argv[1], "r");
    }
    else{
        f1 = fopen(argv[1], "r");
        f2 = fopen(argv[2], "r");
    }
    
    /* Read into ppm */
    Pnm_ppm image1 = Pnm_ppmread(f1, methods);
    Pnm_ppm image2 =  Pnm_ppmread(f2, methods);
    Pnm_rgb pixel1, pixel2;

    int width_diff = image1->width - image2->width;
    int height_diff = image1->height - image2->height;
    int small_w;
    int small_h;

    assert(1 >= abs(width_diff));
    assert(1 >= abs(height_diff));

    if(width_diff >= 0){
        small_w = image2->width;
    }else{
        small_w = image1->width;
    }

    if(height_diff >= 0){
        small_h = image2->height;
    }else{
        small_h = image1->height;
    }

    int r = 0 , g = 0, b = 0;
    double sum = 0;
    int step = 0;
    for (int row = 0; row < small_h; row++){
        for (int col = 0; col < small_w; col++){
            pixel1 = image1->methods->at(image1->pixels, col, row);
            pixel2 = image2->methods->at(image2->pixels, col, row);
            r = pixel1->red - pixel2->red;
            g = pixel1->green - pixel2->green;
            b = pixel1->blue - pixel2->blue;
            step = (r*r + g*g + b*b);
            sum += step;
        }
    }

   double E = sqrt((sum)/(3 * small_w * small_h)) / image1->denominator;
   printf("%.4f\n",E);
   fclose(f1);
   fclose(f2);
   Pnm_ppmfree(&image1);
   Pnm_ppmfree(&image2);

   return 0;
}