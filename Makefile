all: rsa server client
	
rsa:
	g++ -g -lm -o rsa rsa.cpp

server:
	g++ -O2 -lpthread -o server server.cpp csapp.cpp

client:
	g++ -O2 -lpthread -o client client.cpp csapp.cpp

clean:
	rm rsa
	rm server
	rm client

run:
	./rsa
