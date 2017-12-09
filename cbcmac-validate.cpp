#include "util.h"
#include <cstdlib>
#include <cstdio>
#include <string>
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

	cbc_mac(argv[k],argv[m],"cbctest.txt");
	FILE *cbctest, *cbcreal;
	cbctest = fopen("cbctest.txt", "rb");
	cbcreal = fopen(argv[t], "rb");
	long tsize, rsize;
	fseek(cbctest, 0, SEEK_END);
	tsize = ftell(cbctest);
	fseek(cbctest, 0, SEEK_END);
	fseek(cbcreal, 0, SEEK_END);
	rsize = ftell(cbcreal);
	fseek(cbcreal, 0, SEEK_END);
	char *cbctbuff, *cbcrbuff;
	cbctbuff = (char*)malloc(tsize+1);
	cbcrbuff = (char*)malloc(rsize+1);
	fread(cbctbuff, tsize, 1, cbctest);
	fread(cbcrbuff, rsize, 1, cbcreal);

	if(strcmp(cbctbuff , cbcrbuff)==0)
		printf("true\n");
	else
	{
		printf("%d\n", strcmp(cbctbuff , cbcrbuff));
		printf("false\n");
	
	}
	fclose(cbctest);	
	return 0;
}
