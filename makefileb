MFLAGS =  -I/usr/local/include -I/usr/local/Cellar/openssl/1.0.2l/include/  -L/usr/local/Cellar/openssl/1.0.2l/lib/ -L/usr/local/lib -lgmp -lcrypto


mac:
	g++ -x c -o rsa-validate rsa-validate.c $(MFLAGS) 
	g++ -x c -o rsa-sign rsa-sign.c $(MFLAGS)
	g++ -x c -o rsa-keygen rsa-keygen.c $(MFLAGS)

clean:
	rm rsa-dec rsa-enc rsa-keygen *.o
