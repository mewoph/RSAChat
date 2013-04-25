#include <iostream>
#include <stdio.h>
#include <math.h>
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
/*------*/


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
long RSA :: getPrime(long n) {

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




int main(int argc, char *argv[]) {

    long a,b,c,d,e,m;

    char letter;

    RSA *rsa = new RSA();

    cout << "-----IMPLEMENTATION OF RSA ALGORITHM-----";
    cout << endl << endl << "Enter n-th prime:";
    cin >> a;

    cout<<"Enter m-th prime:";
    cin >> b;

    a = rsa->getPrime(a);
    b = rsa->getPrime(b);

    rsa->setProduct(a * b);

    if ( rsa->isPrime(a) && rsa->isPrime(b) ) {

        cout << "a: " << a << " b: " << b << endl;
        c = rsa->multiply(a,b);
        cout << endl << "-----Intmediate Calculations-----";
        cout << endl << "c :"<< c;

        m = rsa->multiply(a-1, b-1);
        cout << endl << "m :" << m;

        e = rsa->findE(m);
        cout << endl << "e :" << e;
        rsa->setPublicKey(e);

        d = rsa->findD(e, m);
        cout << endl << "d :" << d << endl;
        rsa->setPrivateKey(d);

        cout << endl << endl << endl <<"----KEYS----";
        cout << endl << "Public Key : (" << e << "," << c << ")";
        cout << endl << "Private Key : (" << d << "," << c << ")";

        cout << endl << endl << endl << "----ENCRYPTION----" << endl;
        cout << "Enter text to encrypt: ";
        cin >> letter;

        long letterLong = (long)letter;

        long encrypted = rsa->encrypt(letterLong, e, c);

        cout << "Encrypted text: " << encrypted << endl;

        long decrypted = rsa->encrypt(encrypted, d, c);

        cout << "Decrypted Text: " << (char)decrypted << endl;

    } else {

        cout << "Error! Enter two prime numbers!";
    }

    return 0;

}