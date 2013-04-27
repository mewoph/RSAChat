#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string.h>
#include "rsa.h"

using namespace std;

/* Getters and Setters */

long RSA :: getPublicKey() {
    return publicKey;
}

long RSA :: getPrivateKey() {
    return privateKey;
}

long RSA :: getProduct() {
    return product;
}

void RSA :: setPublicKey(long key) {
    publicKey = key;
}

void RSA :: setPrivateKey(long key) {
    privateKey = key;
}

void RSA :: setProduct(long newProduct) {
    product = newProduct;
}

/*---------------------*/


/**
* Determine whether the given number is a prime number
*/
bool RSA :: isPrime(long n) {

    long c = 2;
    if (n < 2) return false;

    while(c <= (n / 2)) {
        if (n % c == 0) return false;
        c++;
    }

    return true;
}

/**
* Get the n-th prime number
*/
long RSA :: getPrime(int n) {

    if (n < 1) {
     cout << "Invalid input" << endl;
    }

    long c = 0; // The current index on the prime list
    long test = 2; // The number we're testing for primality
    while(c < n) {
        if (isPrime(test)) c++;
        test++;
    }

    return test - 1;
}

/*
* Multiply m by n
*/
long RSA :: multiply(long m, long n) {
    return m * n;
}

/**
* Euclid's Algorithm
* Determine whether m and n are coprimes
*/
long RSA :: gcd(long x, long y) {

    while(y != 0) {
        long temp = y;
        y = x % y;
        x = temp;
    }

    return x;
}

/**
* Compute the encyption key
* (coprime to m)
*/
long RSA :: findE(long m) {

    for(long i = 2; i < m; i++) {
        if (gcd(i, m) == 1) return i;
    }

    return 1;
}

/**
* Compute the modular inverse of e mod m
*/
long RSA :: findD(long e, long m) {

    e %= m;

    for(long d = 2; d < m; d++) {
        if((e * d) % m == 1) return d;
    }

    return 1;

}

/**
* Compute (a ^ b) % c
*/
long RSA :: encrypt(long a, long b, long c) {

    long answer = a;

    for(int i = 0; i < b - 1; i++) {
        answer = (answer * a) % c;
    }

    return answer;
}

/**
* Encrypt the given string with the specified key (b,c)
* Return a vector of encrypted characters
*/
vector<long> RSA :: encryptAll(char* string, long b, long c) {

    vector<long> encrypted;

    for(int i = 0; i < strlen(string); i++) {
        long a = encrypt(string[i], b, c);
        encrypted.push_back(a);
        // cout << " " << a;
    }

    return encrypted;
}

/**
* Encrypt the string and store encrypted message in buffer
*/
void RSA :: getEncryptedText(char * string, char* buffer) {

    vector<long> encrypted = encryptAll(string, publicKey, product);

    for(int i = 0; i < encrypted.size(); i++) {
        char strLong[100];
        long l = encrypted.at(i);
        sprintf(strLong, "%ld", l);
        strcat(buffer, " ");
        strcat(buffer, strLong);
    }

}

/**
* Convert the encrypted char * to vector
*/
vector<long> RSA :: toEncryptedVector(char* cipher) {

    vector<long> encrypted;

    char *c;
    c = strtok (cipher," ,.-");

    while (c != NULL) {
        long l = atol(c);
        encrypted.push_back(l);
        c = strtok (NULL, " ,.-");
    }

    return encrypted;
}

/**
* Decrypt the given vector of characters with the speficied key (b, c)
*/
void RSA :: decryptAll(vector<long> encrypted, long b, long c) {

    for(int i = 0; i < encrypted.size(); i++) {
        cout << (char)encrypt(encrypted.at(i), b, c);
    }

    cout << "\n";
}

/**
* Print the decrypted text to console
*/
void RSA :: getDecryptedText(char* cipher) {

    vector<long> encrypted = toEncryptedVector(cipher);
    decryptAll(encrypted, privateKey, product);
}

/**
* Find the prime factors for the specified number
*/
vector<long> RSA :: findPrimeFactors(long c) {

    vector<long> factors;

    int i, j;

    for(i = 1; i < c; i++) {

        for(j = 1; j < c; j++) {

            long a = getPrime(i);
            long b = getPrime(j);

            if (a * b == c) {
                factors.push_back(a);
                factors.push_back(b);
                return factors;
            }
        }
    }

    return factors;

}

/**
* Guess the private key from the specified public key (e, c)
*/
long RSA :: findPrivateKey(long e, long c) {


    vector<long> factors = findPrimeFactors(c);

    long i = factors.at(0);
    long j = factors.at(1);

    long m = multiply(i - 1, j - 1);
    long d = findD(e, m);

    return d;

}
