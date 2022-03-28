/*
 *     40image.c
 *     BY Luca Conetta and Anesu Gavhera 03/22/2021
 *     Comp40 HW4 -  arith
 *
 *     This program provides the main from which compress or decompress are 
 *     called.
 *     Provides a menu interface from which the user can select the relevant 
 *     features they would like to use.
 *
 *     Last Updated: 03/22/2021
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include <string.h>

static void (*compress_or_decompress)(FILE *input) = compress40;

int main(int argc, char *argv[])
{
        int i;

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-c") == 0) {
                        compress_or_decompress = compress40;
                } else if (strcmp(argv[i], "-d") == 0) {
                        compress_or_decompress = decompress40;
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n",
                                argv[0], argv[i]);
                        exit(1);
                } else if (argc - i > 2) {
                        fprintf(stderr, "Usage: %s -d [filename]\n"
                                "       %s -c [filename]\n",
                                argv[0], argv[0]);
                        exit(1);
                } else {
                        break;
                }
        }
        assert(argc - i <= 1);    /* at most one file on command line */
        if (i < argc) {
                FILE *fp = fopen(argv[i], "r");
                assert(fp != NULL);
                compress_or_decompress(fp);
                fclose(fp);
        } else {
                compress_or_decompress(stdin);
        }
        return EXIT_SUCCESS; 
}

