all: server client keys
	
server:
	g++ -O2 -lm -lpthread -g -o server rsa.cpp server.cpp csapp.cpp

client:
	g++ -O2 -lm -lpthread -g -o client rsa.cpp client.cpp csapp.cpp

keys:
	g++ -g -lm -o keys rsa.cpp keys.cpp

clean:
	rm server
	rm client
	rm keys


