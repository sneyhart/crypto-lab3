/* Lab 2 CS483/583  *
 *    rsa-enc.c	    *
 *   Daniel O'Neil  *
 *   Sam Neyhart    */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "gmp.h"

int main(int argc, char **argv)
{
    char inputStr[8192];
    unsigned char outputStr[8192];
    FILE *key ,*in, *out, *rand;
    mpz_t n, e, c, m;
    int i, bits, rbyte; 
    size_t msize;

    assert(argc == 7);
    for(i = 1; i < argc; i++){
	if(strcmp(argv[i], "-k") == 0) key  = fopen(argv[i+1], "r");
	if(strcmp(argv[i], "-i") == 0) in   = fopen(argv[i+1], "r");
	if(strcmp(argv[i], "-o") == 0) out  = fopen(argv[i+1], "w");
    }
    rand = fopen("/dev/random", "r");
    if(key == NULL || in == NULL || out == NULL || rand == NULL) exit(1);
    
    mpz_inits(n,e,c,m,NULL);

    fscanf(key, "%d", &bits);
    rbyte = bits / 16;
    fscanf(key, "%8191s", inputStr);
    mpz_set_str(n,inputStr, 10);

    fscanf(key, "%8191s", inputStr);
    mpz_set_str(e,inputStr, 10);

    fscanf(in, "%8191s", inputStr);
    mpz_set_str(m,inputStr, 10);
    outputStr[0] = 0x01;
    outputStr[1] = 0x02;
    for(i = 2; i < 2 + rbyte; i++){
	do{
	    outputStr[i] = fgetc(rand);
	} while(outputStr[i] == 0);
    }
    outputStr[2 + rbyte] = 0x0;
    mpz_export((void *)(outputStr + 3 + rbyte), &msize, 1, sizeof(char), 0, 0, m);
    mpz_import(m, msize+rbyte+3, 1, sizeof(char), 0, 0, (void *)outputStr);
    
    mpz_powm_sec(c, m, e, n); 
    printf ("c = ");
    mpz_out_str(stdout,10,c);
    printf ("\n");
    mpz_out_str(out,10,c);
    fprintf (out,"\n");
   
    mpz_clears(n,e,c,m,NULL);
    fclose(key); fclose(in); fclose(out);
    return 0;
}
