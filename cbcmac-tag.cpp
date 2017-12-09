#include "util.h"
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <sstream>
#include "cbc-mac.h"

using namespace std;

int main(int argc, char ** argv)
{
	SSL_load_error_strings();
	int k, i, m, t, v, tmp;
	FILE *input, *output, *kf, *mf, *of;

	//command line args
	k = -1, m = -1, t = -1;
	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-k") == 0)
			k = i+1;
		else if(strcmp(argv[i], "-m") == 0)
			m = i+1;
		else if(strcmp(argv[i], "-t") == 0)
			t = i+1;
		else if(argv[i][0] == '-')
			fprintf(stderr, "%s flag not recognized.\n", argv[i]);
	}
	if(k == -1 || t == -1 || m == -1){
		fprintf(stderr, "must specify a key, an input file, and an output file.\n");
		return 1;
	}	
	/*
	kf = fopen(argv[k], "rb");
	mf = fopen(argv[m], "rb");
	of = fopen(argv[t], "rb");
	if(kf == NULL || mf == NULL || of == NULL)
	{		
		fprintf(stderr, "Unable to open the specified files.\n");
		return -1;
	}
	*/

	cbc_mac(argv[k],argv[m],argv[t]);

	printf("it worked\n");
	/*
	fclose(kf);
	fclose(mf);
	fclose(of);
	*/
	return 0;
}
