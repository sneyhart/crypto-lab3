/* Lab 2 CS483/583  *
 *    rsa-dec.c	    *
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
    FILE *key ,*in, *out;
    mpz_t n, d, c, m;
    int i, bits;
    size_t msize;

    assert(argc == 7);
    for(i = 1; i < argc; i++){
	if(strcmp(argv[i], "-k") == 0) key  = fopen(argv[i+1], "r");
	if(strcmp(argv[i], "-i") == 0) in   = fopen(argv[i+1], "r");
	if(strcmp(argv[i], "-o") == 0) out  = fopen(argv[i+1], "w");
    }
    if(key == NULL || in == NULL || out == NULL) exit(1);

    mpz_inits(n,d,c,m,NULL);

    fscanf(key, "%d", &bits);
    fscanf(key, "%8191s", inputStr);
    mpz_set_str(n,inputStr, 10);

    fscanf(key, "%8191s", inputStr);
    mpz_set_str(d,inputStr, 10);

    fscanf(in, "%8191s", inputStr);
    mpz_set_str(c,inputStr, 10);
    
    mpz_powm_sec(m, c, d, n); 
    
    mpz_export((void *)(inputStr), &msize, 1, sizeof(char), 0, 0, m);
    i = 0;
    while(inputStr[i] != 0x0) i++;
    mpz_import(m, msize - i - 1, 1, sizeof(char), 0, 0, (void *)(inputStr + i + 1));
    
    printf ("m = ");
    mpz_out_str(stdout,10,m);
    printf ("\n");
    mpz_out_str(out,10,m);
    fprintf (out,"\n");
   
    mpz_clears(n,d,c,m,NULL);
    fclose(key); fclose(in); fclose(out);
    return 0;
}
