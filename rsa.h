#ifndef RSAH
#define RSAH

#include <vector>

using namespace std;

class RSA {


protected:
	long publicKey;
	long privateKey;
	long product;

public:

	RSA() {}

	long getPublicKey();

	long getPrivateKey();

	long getProduct();

	void setPublicKey(long key);

	void setPrivateKey(long key);

	void setProduct(long newProduct);

	bool isPrime(long n);

	long getPrime(int n);

	long multiply(long m, long n);

	long gcd(long x, long y);

	long findE(long m);

	long findD(long e, long m);

	long encrypt(long a, long b, long c);

	vector<long> encryptAll(char* string, long b, long c);

	void decryptAll(vector<long> encypted, long b, long c);

	vector<long> findPrimeFactors(long c);

	long findPrivateKey(long e, long c);

	void getEncryptedText(char* string, char* buffer);

	vector<long> toEncryptedVector(char* string);

	void getDecryptedText(char* string);


};

#endif
;
