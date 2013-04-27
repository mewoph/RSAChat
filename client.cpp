#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <vector>
#include "csapp.h"
#include "rsa.h"

using namespace std;

RSA* rsaSender;
RSA* rsaReceiver;

/**
* Edit the key associated with the current session
*/
void editKey() {

	cout << "--------------------" << endl << endl;

	cout << "Your recipient's key is currently (" << rsaReceiver->getPublicKey() << ", " << rsaReceiver->getProduct() << ")." << endl << endl;

	long key, n;
	cout << "Enter your recipient's new key: ";
	cin >> key;
	cout << "Enter the new product: ";
	cin >> n;

	rsaReceiver->setPublicKey(key);
	rsaReceiver->setProduct(n);

	cout << "Your recipient's key is now (" << rsaReceiver->getPublicKey() << ", " << rsaReceiver->getProduct() << ")." << endl << endl;

	cout << "--------------------" << endl << endl;

}

/**
* Try to find private key
*/
void findKey() {

	long key, n, guess;

	cout << "--------------------" << endl << endl;
	
	cout << "Enter the public key to guess the private key: ";
	cin >> key;
	cout << "Enter the product associated with the key: ";
	cin >> n;

	guess = rsaSender->findPrivateKey(key, n);

	cout << "Try this key: (" << guess << "," << n << ")" << endl;

	cout << "--------------------" << endl << endl;

}


int main(int argc, char *argv[]) {

	int clientfd, port;
	char *host, buf[1024];
	fd_set rset;

	if (argc != 3) {
		cout << "Usage: ./client <host> <port>"  << endl;
		exit(1);
	}

	host = argv[1];
	port = atoi(argv[2]);

	clientfd = Open_clientfd(host, port);

	if (clientfd < 0) {
		cout << "Error connecting to server." << endl;
		exit(1);
	}

	cout << "Now connected to " << host << endl;
	

	// Set up RSA for the client

	cout << "Let's set up a secure chat." << endl << endl;
	cout << "--------------------" << endl << endl;

	rsaSender = new RSA(); 

	long senderPublicKey, senderPrivateKey, senderProduct;
	cout << "Enter your public key: ";
	cin >> senderPublicKey;
	cout << "Enter your private key: ";
	cin >> senderPrivateKey;
	cout << "Enter the product: ";
	cin >> senderProduct;

	rsaSender->setPublicKey(senderPublicKey);
	rsaSender->setPrivateKey(senderPrivateKey);
	rsaSender->setProduct(senderProduct);

	cout << "Your public key is now (" << rsaSender->getPublicKey() << ", " << rsaSender->getProduct() << ")." << endl << endl;

	rsaReceiver = new RSA(); 

	long receiverPublicKey, receiverProduct;
	cout << "Enter your recipient's public key: ";
	cin >> receiverPublicKey;
	cout << "Enter the product associated with the recipient's key: ";
	cin >> receiverProduct;

	rsaReceiver->setPublicKey(receiverPublicKey);
	rsaReceiver->setProduct(receiverProduct);

	cout << "Your recipient's key is now (" << rsaReceiver->getPublicKey() << ", " << rsaReceiver->getProduct() << ")." << endl << endl;
	cout << "Enter '.quit' to exit program, enter '.keys' to change your keys, enter '.crack' to guess key." << endl << endl;
	cout << "HAPPY CHATTING" << endl << endl;
	cout << "--------------------" << endl << endl;

	while(1) {

		FD_ZERO(&rset);
		FD_SET(0, &rset);
		FD_SET(clientfd, &rset);
		select(clientfd + 1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(0, &rset)) {

			bzero(&buf, sizeof(buf));
			buf[1023] = '\0';

			// get message from user

			cin.getline(buf, sizeof(buf));

			// check for escaped keywords - .quit, .keys, .crack

			if (strstr(buf, ".keys") != NULL) {
				editKey();

			} else if (strstr(buf, ".quit") != NULL) {
				write(clientfd, buf, sizeof(buf));
				cout << "Exit client." << endl;
				Close(clientfd);
				exit(0);

			} else if (strstr(buf, ".crack") != NULL) {
				findKey();


			} else {

				// encrypt message

				char cipher[1024];
	        	bzero(&cipher, sizeof(cipher));
	        	rsaReceiver->getEncryptedText(buf, cipher);
				
				// write encrypted message to server
			
				int bytesWrote = write(clientfd, cipher, sizeof(cipher));

				if (bytesWrote < 0) {
					cout << "Write failed." << endl;
					Close(clientfd);
					exit(0);
				}
			}

		}

		if (FD_ISSET(clientfd, &rset)) {

			bzero(&buf, sizeof(buf));
			buf[1023] = '\0';

			// Read reply from server

			int bytesRead = recv(clientfd, &buf, sizeof(buf), 0);

			if (strstr(buf, ".quit") != NULL) {	
				cout << "Exit client" << endl;
				exit(0);
			} 

			// decrypt message

			cout << "Server: " << buf << endl;
			cout << "Decrypted Message: ";

			rsaSender->getDecryptedText(buf);

			bzero(&buf, sizeof(buf));
		}

	}
	
	Close(clientfd);


}