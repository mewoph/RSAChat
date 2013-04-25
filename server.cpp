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
#include "csapp.h"


pthread_mutex_t mutex;

using namespace std;


/**
* Log requests and responses to file
*/
void logAction(const char* fileName, char* logMessage) {
	FILE *file;
	file = fopen(fileName,"a+"); // open file for append
	fprintf(file,"%s\n",logMessage);
	fclose(file);
}

/**
* Read entire message
* Return the number of bytes read
*/
int readAll(int fd, char* buffer) {

	int bytesRead = 0;
	char c;

	recv(fd, &c, sizeof(c), 0);

	while(c != '\0' && c != '\n') {
		buffer[bytesRead] = c;
		recv(fd, &c, sizeof(c), 0);
		bytesRead++;
	}

	return bytesRead;
}

/**
* Respond to a request
* Adapted from csapp's Tiny Server
*/
void* respondRequest(void* desc) {

	int fd = *(int*)desc;

	char buf[MAXLINE];

	while(1) {

		// Read message from client

		bzero(&buf, sizeof(buf));
		buf[MAXLINE - 1] = '\0';

		int bytesRead = readAll(fd, buf);

		if (bytesRead < 0) {
			cout << "Read failed." << endl;
			exit(0);
		}

		cout << "Client said: " << buf << endl;

		// Quit the server
		if (strstr(buf, ".quit") != NULL) {
			
			printf("Exit server\n");
			exit(0);

		} else {

		// ask user for input for the reply
			cout << "[SERVER] Enter message: ";
			char reply[2048];
			cin >> reply;
			write(fd, reply, strlen(reply));
			bzero(&reply, sizeof(reply));
			buf[MAXLINE - 1] = '\0';
		}
	}

	bzero(&buf, sizeof(buf));
	buf[MAXLINE - 1] = '\0';

}

int main(int argc, char *argv[]) {


	pthread_mutex_init(&mutex, NULL);

	int listenfd, connfd, port, clientlen;
	struct sockaddr_in clientaddr;

	if (argc != 2) {
		cout << "Usage: ./server <port>" << endl;
		exit(1);
	}

	port = atoi(argv[1]);

	listenfd = Open_listenfd(port);

	cout << "Waiting for incoming connection..." << endl;


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