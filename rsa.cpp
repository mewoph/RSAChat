#include <iostream>
#include <stdio.h>
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

/**
* Encrypt the given string with the specified key (b,c)
* Return a vector of encrypted characters
*/
vector<long> RSA :: encryptAll(char* string, long b, long c) {

    vector<long> encrypted;

    for(int i = 0; i < strlen(string); i++) {
        long a = encrypt(string[i], b, c);
        encrypted.push_back(a);
        cout << " " << a;
    }

    cout << "\n";

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
* Guess the private key from the specified public key (e, c)
*/
long RSA :: findPrivateKey(long e, long c) {

    long key;

    // key = (k * phi(c) + 1) / e

    // find phi(c)
    // c is product of two primes
        // find all the prime factors of c
        // try all (?)
    // phi(c) = (a - 1)(b - 1)

    // We know that k * phi(c) + 1 is evenly divisible by e
    // try k * phi(c) + 1 = e, 2e, 3e....

}

int main(int argc, char *argv[]) {

    long a,b,c,d,e,m;

    char letter;

    RSA *rsa = new RSA();

    cout << "-----RSA ENCRYPTION-----";
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
        cout << endl << "-----Intermediate Calculations-----";
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


        cout << "Enter word to encrypt: " << endl;
        char reply[1024];
        cin.ignore();
        cin.getline(reply, sizeof(reply));

        cout << "Response is " << reply << endl;


        vector<long> cipher = rsa->encryptAll(reply, e, c);

        cout << "Decrypted Text: ";
        rsa->decryptAll(cipher, d, c);

    } else {

        cout << "Error! Enter two prime numbers!";
    }

    return 0;

}