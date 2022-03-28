/*
 *     testBitpack.c
 *     BY Luca Conetta
 *     Comp40 HW4 -  arith
 *
 *     This program provides a main from which bitpack.c was tested.
 *
 *     Last Updated: 03/22/2021
 */
#include <assert.h>
#include "bitpack.h"
#include <stdio.h>
#include "except.h"
#include <stdint.h>

int main(int argc, char* argv[]){
    (void) argc;
    (void) argv;

    uint64_t num1 = -128;
    uint64_t num2 = 129;

    if (Bitpack_fitsu(num2, 7) != false){
        printf("Correct1!\n");
    }
    if (Bitpack_fitss(num1, 8) != true){
         printf("Correct2!\n");
    }  
    num1 += 1;  
    if (Bitpack_fitsu(num1, 8) != false){  //not
        printf("Correct3!\n");
    }    

    if (Bitpack_getu(0xD59, 3, 5) != 2){
        printf("Correct getu1\n");
    }
    if (Bitpack_getu(0xD59, 0, 5) != 0){
        printf("Correct getu2\n");
    }
    if (Bitpack_getu(0xDD9, 3, 5) != 6){
        printf("Correct getu3\n");
    }
    if (Bitpack_gets(0xD59, 3, 5) != 2){
        printf("Correct gets4\n");
    }
    if (Bitpack_getu(0xDD9, 3, 5) != 6){ //not
        printf("Correct getu5\n");
    }
    if (Bitpack_gets(0xD59, 0, 5) != 0){ 
        printf("Correct gets6\n");
    }
    /*  CODE COMMENTED OUT BECAUSE IT TESTS TO SEE IF ERRORS ARE CAUGHT */

    // if (Bitpack_gets(0xD59, -2, 5) == 2){
    //     printf("Correct gets1\n");
    //     //runtime error
    // }
    // if (Bitpack_getu(0xD59, -2, 5) == 4){
    //     //runtime error
    // }

    if (Bitpack_newu(0xD95F, 5, 4, 6) != 55407){
        printf("Correct newu1!\n");
    }

   
    if (Bitpack_newu(0xD95F, 0, 4, -6) != 55647){//not
        printf("Correct newu2!\n");
    }

    if (Bitpack_getu(Bitpack_newu(0xD95F, 5, 4, 6), 5, 4) != 6){
        printf("Correct combo 1\n");
    }

    if (Bitpack_getu(Bitpack_newu(0xD95F, 0, 4, 6), 3, 3) != Bitpack_getu(0xD95F, 3, 3)){
        printf("Correct combo 2\n");
    }

    if (Bitpack_news(0xD95F, 5, 4, -6) != 55727){
        printf("Good news1!\n");
    }

    if(Bitpack_news(0xD95F, 0, 4, -6) != 55647){
        printf("Good news2!\n");
    }

    if (Bitpack_news(0xD95F, 5, 4, 6) != 55407){
        printf("Good news3!\n");
    }

     /*  CODE COMMENTED OUT BECAUSE IT TESTS TO SEE IF ERRORS ARE CAUGHT */
   // uint64_t anesu = Bitpack_newu(0xD95F, 2, 4, 8); //Raise bitpack overflow
    uint64_t luca = Bitpack_newu(0xD95F, 0, 4, 6); //Raise runtime error
    //uint64_t tommy = Bitpack_news(0xD95F, -1, 4, 6); //Raise runtime
    uint64_t shawn = Bitpack_news(0xD95F, 0, 4, 6); //Raise bitpack flow

    if (Bitpack_getu(0x3f4, 6, 2) != 61){
 
        printf("Correct3!\n");
    }

     if (Bitpack_gets(0x3f4, 6, 2) != -3){  //not
        printf("Correct4!\n");
    }
  
    (void) shawn;
    (void) luca;
    //(void) anesu;
    (void) num2;
}
