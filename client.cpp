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

using namespace std;

/**
* Read entire message
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
* Adapted from csapp's echo client
http://csapp.cs.cmu.edu/public/ics2/code/netp/echoclient.c
*/

int main(int argc, char *argv[]) {

	int clientfd, port;
	char *host, buf[MAXLINE];

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

	while(1) {

		// Read message from client

		bzero(&buf, sizeof(buf));
		buf[MAXLINE - 1] = '\0';

		int bytesRead = readAll(fd, buf);

		if (bytesRead < 0) {
			cout << "Read failed." << endl;
			exit(0);
		}

		cout << "Server said: " << buf << endl;

		// Quit the server
		if (strstr(buf, ".quit") != NULL) {
			
			printf("Exit server\n");
			exit(0);

		} else {

		// ask user for input for the reply
			cout << "[CLIENT] Enter message: ";
			char reply[2048];
			cin >> reply;
			write(fd, reply, strlen(reply));
			bzero(&reply, sizeof(reply));
		}

	}
	
	Close(clientfd);


}