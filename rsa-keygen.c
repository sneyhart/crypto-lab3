/* Lab 2 CS483/583  *
 *    rsa-keygen.c  *
 *   Daniel O'Neil  *
 *   Sam Neyhart    */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <unistd.h>
#include "gmp.h"
#define print_num(num) {mpz_out_str(stdout,10,num); printf("\n");}
#define fprint_num(file, num) {mpz_out_str(file,10,num); fprintf(file, "\n");}

typedef uint32_t pint;
typedef uint64_t xint;

bool miller_rabin_test(mpz_t d, mpz_t n)
{
	mpz_t a;
	mpz_init(a);
	mpz_t b;
	mpz_init(b);
    	gmp_randstate_t state;
    	gmp_randinit_default(state);
	mpz_sub_ui(b, n, 4);
	mpz_urandomm(a, state, b);
	mpz_add_ui(a,a,2);
	
	mpz_t x;
	mpz_init(x);
	mpz_powm(x, a, d, n);

	mpz_t n_1;
	mpz_init(n_1);
	mpz_set(n_1,n);
	mpz_sub_ui(n,n,1);

	if(mpz_cmp_ui(x,1) || mpz_cmp(x,n_1))
		return true;
	while(mpz_cmp(d, n_1))
	{
		mpz_mul(x,x,x);
		mpz_mod(x,x,n_1);
		mpz_mul_ui(d,d,2);
		if(mpz_cmp_ui(x,1)) 
			return false;		
		if(mpz_cmp(x,n_1))
			return true;
	}
	return false;


}

void find_prime(mpz_t i, mpz_t j)
{
	mpz_t p;
	mpz_init(p);
	mpz_set(p,j);
	mpz_t n_1;
	mpz_init(n_1);
	mpz_sub_ui(n_1, j, 1);

	mpz_t d;
	mpz_init(d);
	mpz_set(d,n_1);
	
	mpz_t m;
	mpz_init(m);
	mpz_mod_ui(m,n_1,2);

	while(mpz_cmp_ui(m,0) == 0)
	{	
		mpz_div_ui(d,d,2);
		mpz_mod_ui(m,d,2);
	}



	while(miller_rabin_test(d, p) == 0)
		mpz_add_ui(p,p,1);

	mpz_set(i,p);
}

int main(int argc, char **argv)
{
    char inputStr[8];
    char *cert, *pubk, *privk;
    FILE *pub ,*sec, *seed;
    mpz_t p, q, t, n, e, d;
    gmp_randstate_t state;
    mp_bitcnt_t bits;
    int i;
    
    for(i = 1; i < argc; i++){
	if(strcmp(argv[i], "-p") == 0){	
	    pub	= fopen(argv[i+1], "w");
	    pubk = argv[i+1];
	}
	if(strcmp(argv[i], "-s") == 0){	
	    sec	= fopen(argv[i+1], "w");
	    privk = argv[i+1];
	}
	if(strcmp(argv[i], "-n") == 0)	bits	= (mp_bitcnt_t)atoi(argv[i+1]);
	if(strcmp(argv[i], "-c") == 0)	cert	= argv[i+1];
    }
    assert(pub != NULL);
    assert(sec != NULL);
    assert(bits != 0);
    
    //initialize necessary variables
    mpz_inits(p,q,t,n,e,d,NULL);
    gmp_randinit_default(state);
    seed = fopen("/dev/urandom", "r");
    fread(inputStr, 8, 1, seed);
    gmp_randseed_ui(state, *((unsigned int*)inputStr));

    //get p, q, and n
    do{
	mpz_urandomb(q, state, bits/2);
	mpz_nextprime(p,q);
	//find_prime(p,q);
	mpz_ui_pow_ui(e, 2, bits-1);
	mpz_ui_pow_ui(d, 2, bits);
	mpz_cdiv_q(t, e, p);	//keep t
	mpz_fdiv_q(n, d, p);
	mpz_sub(e, n, t);
	mpz_urandomm(d, state, e);
	mpz_add(e, d, t);
	mpz_nextprime(q, e);
	//find_prime(q,e);
	mpz_mul(n, p, q);
	printf("p = ");	print_num(p);
	printf("q = ");	print_num(q); 
	printf("n = ");	print_num(n); 
	printf("n is %zu bits.\n\n", mpz_sizeinbase(n,2));
    }while(mpz_sizeinbase(n,2) != bits || mpz_probab_prime_p(p, 15) < 1 || mpz_probab_prime_p(q,15) < 1);
   
    //get totient of p and q
    mpz_sub_ui(e, p, 1);
    mpz_sub_ui(d, q, 1);
    mpz_lcm(t, e, d); 
    printf("t = "); print_num(t);

    //pick e
    mpz_set_str(e, "65537", 10);
    printf("e = "); print_num(e);

    //solve for d
    mpz_invert(d, e, t);
    printf("d = "); print_num(d);

    //output to files
    fprintf(pub, "%lu\n", bits);
    fprint_num(pub, n);
    fprint_num(pub, e);
    fprintf(sec, "%lu\n", bits);
    fprint_num(sec, n);
    fprint_num(sec, d);
    
    //close files
    fclose(seed); fclose(pub); fclose(sec);
    gmp_randclear(state);
    mpz_clears(p,q,t,n,e,d,NULL);
    
    if(cert == NULL) cert = privk;
    char sig[256];
    strcpy(sig, pubk);
    strcat(sig, "-casig");
    //printf("cert %s\n sig: %s\n", cert, sig);
    if(fork() == 0){
	int file = open("/dev/null", O_APPEND | O_WRONLY);
	dup2(file, 1);
	close(file);
	execl("rsa-sign", "rsa-sign", "-m", pubk, "-k", cert, "-s", sig, (char*)0);
	exit(1);
    }
    wait(&i);
    return 0;
}
