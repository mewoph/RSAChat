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
#include "csapp.h"
#include "rsa.h"

using namespace std;

RSA* rsa;

/**
* Edit the key associated with the current session
*/
void editKey() {

	cout << "--------------------" << endl << endl;

	cout << "Public key is currently (" << rsa->getPublicKey() << ", " << rsa->getProduct() << ")." << endl << endl;

	long key, n;
	cout << "Enter your new key: ";
	cin >> key;
	cout << "Enter the new product: ";
	cin >> n;

	rsa->setPublicKey(key);
	rsa->setProduct(n);

	cout << "Public key is now (" << rsa->getPublicKey() << ", " << rsa->getProduct() << ")." << endl << endl;

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

	guess = rsa->findPrivateKey(key, n);

	cout << "Try this key: (" << guess << "," << n << ")" << endl;

	cout << "--------------------" << endl << endl;

}

/**
* Respond to a request
* Adapted from http://www.thiyagaraaj.com/tutorials/unix-network-programming-example-programs/tcp-chat-programming-in-unix-using-c-programmin
*/
void* respondRequest(void* desc) {

	int fd = *(int*)desc;

	fd_set rset;

	char buf[1024];

	while(1) {

		FD_ZERO(&rset);
		FD_SET(0, &rset);
		FD_SET(fd, &rset);
		select(fd + 1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(0, &rset)) {

			// get message from user

			bzero(&buf, sizeof(buf));
			buf[1023] = '\0';
        	cin.getline(buf, sizeof(buf));

        	// check for escaped keywords - .quit, .keys, .crack

			if (strstr(buf, ".keys") != NULL) {
				editKey();

			} else if (strstr(buf, ".quit") != NULL) {	
				write(fd, buf, sizeof(buf));
				cout << "Exit server" << endl;
				Close(fd);
				exit(0);

			} else if (strstr(buf, ".crack") != NULL) {
				findKey();


			} else {

				// encrypt message

				char cipher[1024];
	        	bzero(&cipher, sizeof(cipher));
	        	rsa->getEncryptedText(buf, cipher);

				// write encrypted message to client

				int bytesWrote = write(fd, cipher, sizeof(cipher));

				if (bytesWrote < 0) {
					cout << "Write failed." << endl;
					Close(fd);
					exit(0);
				}

			}

		} 

		if (FD_ISSET(fd, &rset)) {
		 	
		 	// read message from client

			bzero(&buf, sizeof(buf));
			buf[1023] = '\0';

			int bytesRead = recv(fd, &buf, sizeof(buf), 0);

			if (bytesRead < 0) {
				cout << "Read failed." << endl;
				Close(fd);
				exit(0);
			}

			if (strstr(buf, ".quit") != NULL) {	
				cout << "Exit server" << endl;
				Close(fd);
				exit(0);
			}

			// decrypt message 

			cout << "Client: " << buf << endl;
			cout << "Decrypted Message: ";
			rsa->getDecryptedText(buf);

		 } 
		
	}

}

int main(int argc, char *argv[]) {


	int listenfd, connfd, port, clientlen;
	struct sockaddr_in clientaddr;

	if (argc != 2) {
		cout << "Usage: ./server <port>" << endl;
		exit(1);
	}

	port = atoi(argv[1]);

	listenfd = Open_listenfd(port);


	// Set up RSA for the server

	cout << "Let's set up a secure chat." << endl << endl;
	cout << "--------------------" << endl << endl;
	rsa = new RSA(); 
	long key, n;
	cout << "Enter your key: ";
	cin >> key;
	cout << "Enter the product: ";
	cin >> n;

	rsa->setPublicKey(key);
	rsa->setProduct(n);

	cout << "Public key is now (" << rsa->getPublicKey() << ", " << rsa->getProduct() << ")." << endl << endl;
	cout << "Enter '.quit' to exit program, enter '.keys' to change your keys, enter '.crack' to guess key." << endl << endl;
	cout << "HAPPY CHATTING" << endl << endl;
	cout << "--------------------" << endl << endl;

	// Spawn thread to interact with client

	while(1) {

		pthread_t thread;
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, (SA*)&clientaddr, (socklen_t*) &clientlen);

		if (pthread_create(&thread, NULL, respondRequest, &connfd) < 0) {
			cout << "ERROR CREATING THREAD" << endl;
			return 1;
		}
	}

	Close(connfd);

}