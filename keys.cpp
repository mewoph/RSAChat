#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string.h>
#include "rsa.h"

using namespace std;


int main(int argc, char *argv[]) {

    int a, b;
    long c,d,e,m;

    char letter;

    RSA *rsa = new RSA();

    cout << "-----RSA ENCRYPTION-----";
    cout << endl << endl << "Enter n-th prime: ";
    cin >> a;

    cout<<"Enter m-th prime: ";
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

        cout << "Enter message to encrypt: ";
        char reply[1024];
        cin.ignore();
        cin.getline(reply, sizeof(reply));

        char cipher[1024];
        bzero(&cipher, sizeof(cipher));

        rsa->getEncryptedText(reply, cipher);

        cout << "Encypted Message :" << cipher << endl;


        cout << "Decrypted Message: ";
        rsa->getDecryptedText(cipher);


    } else {

        cout << "Error! Enter two numbers!";
    }

    return 0;
}