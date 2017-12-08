#include <string>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#define ABORT() (fprintf(stderr, "%s\nAborting in %s at %s:%d\n",\
            ERR_error_string(ERR_get_error(), NULL), __PRETTY_FUNCTION__, __FILE__, __LINE__), abort(), 0)
#define BLOCKSIZE 16

#ifndef _UTIL_H_
#define _UTIL_H_

typedef std::basic_string<unsigned char> u_string;
typedef unsigned char u_char;
void encode_128(u_char *key, u_char *in, u_char *out);
void decode_128(u_char *key, u_char *in, u_char *out);
void cpy_128(u_char *dest, u_char *src);
void xor_128(u_char *message, u_char *prev);
u_string encode(u_string key, u_string data);
u_string decode(u_string key, u_string data);

#endif
