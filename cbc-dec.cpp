#include "util.h"
#include <cstdio>
#include <cstdlib>

int main(int argc, char ** argv)
{
	SSL_load_error_strings();
	int k, i, in, o, v, tmp;
	FILE *input, *output, *kf, *iv;

	//command line args
	k = -1, in = -1, o = -1, v = -1;
	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-k") == 0)
			k = i+1;
		else if(strcmp(argv[i], "-i") == 0)
			in = i+1;
		else if(strcmp(argv[i], "-o") == 0)
			o = i+1;
		else if(strcmp(argv[i], "-v") == 0)
			v = i+1;
		else if(argv[i][0] == '-')
			fprintf(stderr, "%s flag not recognized.\n", argv[i]);
	}
	if(k == -1 || in == -1 || o == -1){
		fprintf(stderr, "must specify a key, an input file, and an output file.\n");
		return 1;
	}

	//open files
	kf = fopen(argv[k],"rb");
	input = fopen(argv[in],"rb");
	output = fopen(argv[o],"wb");
	if(kf == NULL || input == NULL || output == NULL){
		fprintf(stderr, "Unable to open the specified files.\n");
		return 1;
	}
	if(v != -1){
		iv = fopen(argv[v],"rb");
	}
	if(iv == NULL){
		fprintf(stderr, "Unable to open the specified files.\n");
		return 1;
	}
	
	//decryption
	u_char key[16];
	u_char prev[16];
	u_char buf[17];
	u_char buf2[17];
	if(fread(&key,1,16,kf) != 16)
		fprintf(stderr,"Not enought bytes read for the key.\n");
	fread(&prev, 1, 16, input);
	tmp = fread(&buf,1,16,input); 
	int pad = 1;
	while(tmp == 16){
		decode_128(key, buf, buf2);
		xor_128(buf2,prev);
		cpy_128(prev,buf);
		buf2[16] = '\0';
		if(buf2[15] > (u_char)0 && buf2[15] <= (u_char)16){
			for(i = 0; i < (int)buf2[15]; i++){
				if (buf2[15-i] != buf2[15]){
					pad = 0;
				}
			}
			if(pad == 1){
				pad = (int)buf2[15];
				//fwrite(buf2,1,16-pad,stdout);
				fwrite(buf2,1,16-pad,output);
				break;
			}
		}
		//printf("%s",buf2);
		//fwrite(buf2,1,16,stdout);
		fwrite(buf2,1,16,output);
		//fprintf(output, "%s",buf2);
		tmp = fread(&buf,1,16,input); 
	}

	printf("File written.\n");
	fclose(kf);
	fclose(input);
	fclose(output);
	return 0;
}
