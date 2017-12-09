CC = g++
CFLAGS = -g -Wall
LFLAGS = -g -Wall
OBJS = cbcmac-tag.o cbcmac-validate.o util.o cbc-mac.o
LIBS = -lssl -lcrypto -lpthread 
MFLAGS =  -I/usr/local/include -I/usr/local/Cellar/openssl/1.0.2l/include/  -L/usr/local/Cellar/openssl/1.0.2l/lib/ -L/usr/local/lib -lgmp -lcrypto

.suffixes: .c .o

all: cbcmac-tag cbcmac-validate $(OBJS) rsa-validate rsa-sign rsa-keygen

.c.o: util.h util.o
	$(CC) $(CFLAGS) -c $*.c

cbcmac-tag: cbcmac-tag.o util.o util.h cbc-mac.o cbc-mac.h
	$(CC) $(LFLAGS) -o $@ $@.o util.o cbc-mac.o $(LIBS)

cbcmac-validate: cbcmac-validate.o util.o util.h cbc-mac.o cbc-mac.h
	$(CC) $(LFLAGS) -o $@ $@.o util.o cbc-mac.o $(LIBS)

rsa-validate: rsa-validate.c
	g++ -x c -o rsa-validate rsa-validate.c $(MFLAGS) 

rsa-sign: rsa-sign.c
	g++ -x c -o rsa-sign rsa-sign.c $(MFLAGS)
	
rsa-keygen: rsa-keygen.c
	g++ -x c -o rsa-keygen rsa-keygen.c $(MFLAGS)

clean:
	rm rsa-sign rsa-validate rsa-keygen *.o
	rm *.o cbcmac-tag cbcmac-validate
	
