/* Lab 2 CS483/583  *
 *    rsa-enc.c	    *
 *   Daniel O'Neil  *
 *   Sam Neyhart    */

#include "openssl/sha.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "gmp.h"

void sha_file(FILE *message, char outputBuffer[65])
{

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    const int bufSize = 32768;
    unsigned char *buffer = (unsigned char *)malloc(bufSize);
    int bytesRead = 0, i;
    while((bytesRead = fread(buffer, 1, bufSize, message)))
    {
        SHA256_Update(&sha256, buffer, bytesRead);
    }
    SHA256_Final(hash, &sha256);
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
    printf("sha256sum: %s\n",outputBuffer);
    free(buffer);
}

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
	if(strcmp(argv[i], "-m") == 0) in   = fopen(argv[i+1], "rb");
	if(strcmp(argv[i], "-s") == 0) out  = fopen(argv[i+1], "w");
    }
    rand = fopen("/dev/random", "r");
    if(key == NULL || in == NULL || out == NULL || rand == NULL){
	fprintf(stderr, "NULL FILE\n");
	exit(1);
    }
    
    //testing
    char ob[65];
    sha_file(in, ob);

    mpz_inits(n,e,c,m,NULL);

    fscanf(key, "%d", &bits);
    rbyte = bits / 16;
    fscanf(key, "%8191s", inputStr);
    mpz_set_str(n,inputStr, 10);

    fscanf(key, "%8191s", inputStr);
    mpz_set_str(e,inputStr, 10);

    //fscanf(in, "%8191s", inputStr);
    mpz_set_str(m, ob, 16);
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
