/* Lab 2 CS483/583  *
 *    rsa-dec.c	    *
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
    //printf("sha256sum: %s\n",outputBuffer);
    free(buffer);
}

int main(int argc, char **argv)
{
    char inputStr[8192];
    FILE *key ,*in, *mes;
    mpz_t n, d, c, m;
    int i, bits, out;
    size_t msize;

    assert(argc == 7);
    for(i = 1; i < argc; i++){
	if(strcmp(argv[i], "-k") == 0) key  = fopen(argv[i+1], "r");
	if(strcmp(argv[i], "-s") == 0) in   = fopen(argv[i+1], "r");
	if(strcmp(argv[i], "-m") == 0) mes  = fopen(argv[i+1], "rb");
    }
    if(key == NULL || in == NULL || mes == NULL) exit(1);
    
    char ob[65];
    sha_file(mes, ob);

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
    
    mpz_set_str(c, ob, 16);
    out = mpz_cmp(c, m);
    if(out == 0) printf("True\n");
    else printf("False\n");

    mpz_clears(n,d,c,m,NULL);
    fclose(key); fclose(in); fclose(mes);
    return 0;
}
