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

			// write message

			bzero(&buf, sizeof(buf));
			buf[1023] = '\0';
        	cin.getline(buf, sizeof(buf));
			
			int bytesWrote = write(fd, buf, sizeof(buf));

			if (bytesWrote < 0) {
				cout << "Write failed." << endl;
				exit(0);
			}
			

		} 

		if (FD_ISSET(fd, &rset)) {
		 	
		 	// read message

			bzero(&buf, sizeof(buf));
			buf[1023] = '\0';

			int bytesRead = recv(fd, &buf, sizeof(buf), 0);

			if (bytesRead < 0) {
				cout << "Read failed." << endl;
				exit(0);
			}

			if (strstr(buf, ".quit") != NULL) {	
				printf("Exit server\n");
				exit(0);
			} 

			cout << "\nClient said: " << buf << endl;

		 } 
		
	}

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