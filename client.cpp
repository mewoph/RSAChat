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

	while(1) {

		FD_ZERO(&rset);
		FD_SET(0, &rset);
		FD_SET(clientfd, &rset);
		select(clientfd + 1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(0, &rset)) {

			bzero(&buf, sizeof(buf));
			buf[1023] = '\0';

			cin.getline(buf, sizeof(buf));
		
			int bytesWritten = write(clientfd, buf, sizeof(buf));

			if (strstr(buf, ".quit") != NULL) {
				Close(clientfd);
				exit(0);
			}


		}

		if (FD_ISSET(clientfd,&rset)) {

			bzero(&buf, sizeof(buf));
			buf[1023] = '\0';

			// Read reply from server

			int bytesRead = recv(clientfd, &buf, sizeof(buf), 0);

			printf("Server: %s\n", buf);

			bzero(&buf, sizeof(buf));
	}

	}
	
	Close(clientfd);


}