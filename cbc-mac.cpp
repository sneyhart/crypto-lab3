#include "cbc-mac.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

char* cbc_mac(char* kn, char* mn, char* tn)
{
	SSL_load_error_strings();
	int k, i, in, o, v, tmp;
	FILE *input, *output, *kf, *iv;

	//open files
	kf = fopen(kn,"rb");
	input = fopen(mn,"rb");
	output = fopen(tn,"wb");
	if(kf == NULL || input == NULL || output == NULL){
		fprintf(stderr, "Unable to open the specified files.\n");
		return NULL;
	}

	long size;
	string prependedm;
	
	//find message file length
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	fseek(input, 0, SEEK_SET);
	stringstream ss;
	ss << size;
	prependedm = ss.str();

	//prepend length to message
	//
	//creates newm file for new message since
	//i didnt want to overwrite old
	char* pbuff;
	pbuff = (char*)malloc(size+1);
	fread(pbuff, size, 1, input);
	rewind(input);
	prependedm.append(pbuff);
	FILE *f = fopen("newm.txt", "rwb");
	fprintf(f, "%s\n", prependedm.c_str());
	//encryption
	time_t t;
	srand((unsigned) time(&t));
	u_char key[16];
	u_char prev[16];
	u_char buf[17];
	u_char buf2[17];
	for(i = 0; i < 16; i++){
		prev[i] = '\0';
	}
	if(fread(&key,1,16,kf) != 16)
		fprintf(stderr,"Not enough bytes read for the key.\n");
	tmp = fread(&buf,1,16,input); 
	while(tmp == 16){
		xor_128(buf,prev);
		encode_128(key, buf, buf2);
		cpy_128(prev,buf2);
		buf2[16] = '\0';
//		printf("%s\n",buf2);
//		fwrite(buf2,1,16,output);
		tmp = fread(&buf,1,16,input); 
	}
	for(i = tmp; i < 16; i++){
		buf[i] = (u_char) (16 - tmp);
	}
	xor_128(buf,prev);
	encode_128(key, buf, buf2);
	buf2[16] = '\0';
//	printf("%s\n",buf2);
	fwrite(buf2,1,16,output);

	

	printf("File written.\n");
	char* ret;
	long osize;
	fseek(output, 0, SEEK_END);
	osize = ftell(input);
	fseek(output, 0, SEEK_SET);
	rewind(output);
	ret = (char*)malloc(osize+1);
	fread(ret, osize, 1, output);
	
	fclose(kf);
	fclose(input);
	fclose(output);
	fclose(f);
	return ret;
}
